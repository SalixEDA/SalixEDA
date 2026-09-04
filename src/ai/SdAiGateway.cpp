/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  The SdAiGateway class handles asynchronous, non-blocking communication with
  OpenAI-compatible AI APIs (including local Ollama instances) within a dedicated worker thread.
  It processes streaming text responses via Server-Sent Events (SSE), parsing the raw HTTP data chunks on the fly.
  Each network request increments a unique transaction identifier, allowing the UI layer
  to correctly route incoming text fragments even during rapid user interactions or cancellation scenarios.
  It supports live generation cancellation via a dedicated abort slot and reads server settings
  seamlessly from the application configuration.
*/
#include "SdConfig.h"
#include "SdAiGateway.h"

#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QNetworkRequest>
#include <QThread>

SdAiGateway::SdAiGateway(QObject *parent) : QObject(parent) {
  // Initialize the manager in the scope of the thread this object lives in.
  mManager = new QNetworkAccessManager(this);
  }




SdAiGateway::~SdAiGateway() {
  cancel();
  }




SdAiGateway* SdAiGateway::instance() {
  // Thread-safe initialization guaranteed by C++11 magic statics.
  static SdAiGateway* singleInstance = nullptr;

  if( singleInstance == nullptr ) {
    // Create the instance without a parent so it can be moved to another thread.
    singleInstance = new SdAiGateway();

    // Create a dedicated worker thread for running network operations.
    QThread* aiThread = new QThread();

    // Move the network gateway object instance to the new worker thread context.
    singleInstance->moveToThread(aiThread);

    // Ensure proper memory cleanup when the application finishes and thread stops.
    connect(aiThread, &QThread::finished, singleInstance, &QObject::deleteLater);
    connect(aiThread, &QThread::finished, aiThread, &QObject::deleteLater);

    // Start the event loop inside the worker thread.
    aiThread->start();
    }

  return singleInstance;
  }


void SdAiGateway::question(const QString &system, const QStringList &dialog) {
  // Safe cleanup of any leftover or hanging active streams.
  cancel();

  // Increment the request identity counter for the new generation session.
  mCurrentId++;

  // Load backend configuration parameters from QSettings.
  QSettings settings;
  QString urlStr = settings.value( SDK_AI_URL, "http://localhost:11434/v1").toString();
  QString apiKey = settings.value( SDK_AI_KEY, "").toString();
  QString modelName = settings.value( SDK_AI_MODEL, "gemma2:2b").toString();

  // Append standard OpenAI chat completions endpoint if missing.
  QUrl url(urlStr);
  if( !url.path().endsWith("/chat/completions") ) {
    url.setPath(url.path() + "/chat/completions");
    }

  // Construct standard HTTP headers.
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  if( !apiKey.isEmpty() ) {
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());
    }

  // Build the payload payload matching OpenAI API standards.
  QJsonObject rootObj;
  rootObj["model"] = modelName;
  rootObj["stream"] = true;

  QJsonArray messagesArray;

  // Append system prompt instructions if provided.
  if( !system.isEmpty() ) {
    QJsonObject sysMsg;
    sysMsg["role"] = "system";
    sysMsg["content"] = system;
    messagesArray.append(sysMsg);
    }

  // Parse the dialog array mapping even items to user and odd to assistant.
  for( int i = 0; i < dialog.size(); ++i ) {
    if( dialog[i].isEmpty() ) {
      continue;
      }
    QJsonObject msg;
    msg["role"] = (i % 2 == 0) ? "user" : "assistant";
    msg["content"] = dialog[i];
    messagesArray.append(msg);
    }

  rootObj["messages"] = messagesArray;
  QByteArray jsonData = QJsonDocument(rootObj).toJson(QJsonDocument::Compact);

  mBuffer.clear();

  // Trigger the asynchronous HTTP POST request.
  mCurrentReply = mManager->post(request, jsonData);

  // Connect network events to internal processing slots.
  connect(mCurrentReply, &QNetworkReply::readyRead, this, &SdAiGateway::onReadyRead);
  connect(mCurrentReply, &QNetworkReply::finished, this, &SdAiGateway::onFinished);
  connect(mCurrentReply, &QNetworkReply::errorOccurred, this, &SdAiGateway::onError);
  }





void SdAiGateway::userQuestion(const QStringList &dialog)
  {

  }




void SdAiGateway::cancel() {
  if( mCurrentReply ) {
    // Disconnect signals to prevent processing trailing bytes or double-completion events.
    mCurrentReply->disconnect(this);
    mCurrentReply->abort();
    mCurrentReply->deleteLater();
    mCurrentReply = nullptr;

    // Notify the UI layer that the generation process for this ID is stopped.
    emit answer(mCurrentId, "");
    }
  }



void SdAiGateway::onReadyRead() {
  if( !mCurrentReply ) {
    return;
    }

  // Read raw SSE packet content and append to internal buffer.
  mBuffer += QString::fromUtf8(mCurrentReply->readAll());
  QStringList lines = mBuffer.split("\n");

  // Preserve the last line fragment which might still be incomplete.
  mBuffer = lines.takeLast();

  for( const QString &line : lines ) {
    QString trimmed = line.trimmed();
    if (trimmed.isEmpty()) {
      continue;
      }

    // Handle standard stream completion payload marker.
    if( trimmed == "data: [DONE]" ) {
      emit answer(mCurrentId, "");
      return;
      }

    // Parse valid OpenAI format data lines starting with data: prefix.
    if( trimmed.startsWith("data:") ) {
      QString jsonStr = trimmed.mid(5).trimmed();
      QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
      if( doc.isNull() ) {
        continue;
        }

      // Extract choices[0].delta.content value from response.
      QJsonObject root = doc.object();
      if( root.contains("choices") && root["choices"].isArray() ) {
        QJsonArray choices = root["choices"].toArray();
        if( !choices.isEmpty() ) {
          QJsonObject firstChoice = choices.at(0).toObject();
          if( firstChoice.contains("delta") && firstChoice["delta"].isObject() ) {
            QJsonObject delta = firstChoice["delta"].toObject();
            if( delta.contains("content") ) {
              QString textChunk = delta["content"].toString();
              if( !textChunk.isEmpty() ) {
                emit answer(mCurrentId, textChunk);
                }
              }
            }
          }
        }
      }
    }
  }





void SdAiGateway::onFinished() {
  if( mCurrentReply ) {
    mCurrentReply->deleteLater();
    mCurrentReply = nullptr;
    }
  // Send empty string to indicate natural generation cycle end.
  emit answer(mCurrentId, "");
  }




void SdAiGateway::onError(QNetworkReply::NetworkError code) {
  Q_UNUSED(code);
  if( mCurrentReply ) {
    // Send network details out as a plain-text payload entry before closing.
    emit answer(mCurrentId, QString("\n[API Error: %1]\n").arg(mCurrentReply->errorString()));
    mCurrentReply->deleteLater();
    mCurrentReply = nullptr;
    }
  emit answer(mCurrentId, "");
  }

