/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
*/
#include "SdGuiderCapture.h"
#include "SvLib/SvJsonIO.h"

#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>
#include <QWidget>
#include <QFileDialog>
#include <QCursor>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QEnterEvent>
#include <QCursor>
#include <QGuiApplication>
#include <QPainter>
#include <QSettings>

SdGuiderCapture::SdGuiderCapture(QWidget *main, QObject *parent) :
  QObject(parent),
  //mFile(nullptr),
  mMainWindow(main)
  {
  QCoreApplication::instance()->installEventFilter( this );

  // connect( &mTimer, &QTimer::timeout, this, [this] () {
  //   if( mFile ) {
  //     //Capture current screen and append it to file
  //     QPoint p = mMainWindow->pos();
  //     QSize s = mMainWindow->size();
  //     QPixmap pix = QGuiApplication::primaryScreen()->grabWindow( 0, p.x(), p.y(), s.width(), s.height() );
  //     mFile->addImage( pix.toImage().convertToFormat(QImage::Format_ARGB32), mTime );
  //     QPoint c = QCursor::pos( QGuiApplication::primaryScreen() );
  //     mFile->addCursor( QGuiApplication::mouseButtons(), QPoint( c.x() - p.x(), c.y() - p.y() ), mTime );
  //     //Next time
  //     mTime++;
  //     //If movie too long we automatic stop capture
  //     if( mTime > 6000 )
  //       captureStop();
  //     }
  //   });
  }

void SdGuiderCapture::setScena(const QString &scriptPath, int scenaIndex, const SdGuiderStepList &list)
  {
  QSettings s;
  mStepList   = list;
  QString eventName("scena-%1.events");
  mEventPath = scriptPath + eventName.arg(scenaIndex);
  mVideoPath = scriptPath + QString("scena-%1-%2.mp4").arg(scenaIndex).arg( s.value(SDK_LANGUAGE).toString() );
  QFile eventFile( mEventPath );
  mEventList.clear();
  if( eventFile.exists() ) {
    //Load existings events
    if( eventFile.open(QIODevice::ReadOnly) ) {
      QJsonObject obj( svJsonObjectFromByteArray(eventFile.readAll()) );
      SvJsonReader js( obj );
      js.jsonList( js, "EventList", mEventList );
      }
    }
  mEvent.mStepIndex = 0;
  }




bool SdGuiderCapture::eventFilter(QObject *watched, QEvent *event)
  {
  Q_UNUSED(watched)
  switch (event->type()) {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick: {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
      mEvent.mMousePosX = mouseEvent->globalPosition().x();
      mEvent.mMousePosY = mouseEvent->globalPosition().y();
      mEvent.mMouseButtons = mouseEvent->buttons();
      mEvent.mKeyModifier = mouseEvent->modifiers();
      break;
      }

    case QEvent::KeyPress:
    case QEvent::KeyRelease: {
      QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
      mEvent.mKeyCode = keyEvent->key();
      mEvent.mKeyChar = keyEvent->text().isEmpty() ? 0 : keyEvent->text().at(0).unicode();
      mEvent.mKeyModifier = keyEvent->modifiers();

      // Для событий клавиатуры также обновляем позицию мыши
      QPoint globalPos = QCursor::pos();
      mEvent.mMousePosX = globalPos.x();
      mEvent.mMousePosY = globalPos.y();
      mEvent.mMouseButtons = QGuiApplication::mouseButtons();
      break;
      }

    // case QEvent::Wheel: {
    //   QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
    //   mEvent.mMousePosX = wheelEvent->globalX();
    //   mEvent.mMousePosY = wheelEvent->globalY();
    //   mEvent.mMouseButtons = wheelEvent->buttons();
    //   mEvent.mKeyModifier = wheelEvent->modifiers();
    //   // Для колесика мыши можно установить специальный код клавиши
    //   mEvent.mKeyCode = 0;
    //   mEvent.mKeyChar = 0;
    //   break;
    //   }

    case QEvent::Enter:
    case QEvent::Leave: {
      QEnterEvent *enterEvent = static_cast<QEnterEvent*>(event);
      mEvent.mMousePosX = enterEvent->globalPosition().x();
      mEvent.mMousePosY = enterEvent->globalPosition().y();
      mEvent.mMouseButtons = QGuiApplication::mouseButtons();
      mEvent.mKeyModifier = QGuiApplication::keyboardModifiers();
      break;
      }

    default:
      break;
    //   // Для других типов событий просто обновляем текущее состояние
    //   QPoint globalPos = QCursor::pos();
    //   mEvent.mMousePosX = globalPos.x();
    //   mEvent.mMousePosY = globalPos.y();
    //   mEvent.mMouseButtons = QGuiApplication::mouseButtons();
    //   mEvent.mKeyModifier = QGuiApplication::keyboardModifiers();
    //   break;
    }

  // Возвращаем false, чтобы не блокировать дальнейшую обработку события
  return false;
  }




void SdGuiderCapture::periodicRecord()
  {
  if( mEventList.size() > 0 && mEventList.last().mStepIndex != 0 && mEvent.mStepIndex == 0 )
    mEventList.clear();
  //Limit scene size for 5min
  if( mEventList.size() < 15000 )
    mEventList.append( mEvent );
  }




void SdGuiderCapture::recordPauseResume()
  {
  if( mRecordTimer.isActive() )
    mRecordTimer.stop();
  else
    mRecordTimer.start(20);
  }




void SdGuiderCapture::nextStep()
  {
  mEvent.mStepIndex++;
  if( mEvent.mStepIndex >= mStepList.count() ) {
    mRecordTimer.stop();
    //mEvent.mStepIndex = 0;
    QFile eventFile( mEventPath );
    //Save events
    if( eventFile.open(QIODevice::WriteOnly) ) {
        QJsonObject obj( svJsonObjectFromByteArray(eventFile.readAll()) );
      SvJsonWriter js;
      js.jsonList( js, "EventList", mEventList );
      eventFile.write( svJsonObjectToByteArray( js.object() )  );
      }
    }
  emit stepIndexChanged( mEvent.mStepIndex );
  }





void SdGuiderCapture::periodicPayer()
  {
  mPlayer.inject( mEventList.at(mEventIndex++) );
  //Get
  if( mCapture ) {
    QPixmap pixmap = mMainWindow->grab();
    QImage image = pixmap.toImage();

    // Создаем новое изображение с увеличенной высотой
    int newHeight = image.height() + 200;
    QImage newImage(image.width(), newHeight, QImage::Format_ARGB32);
    newImage.fill(Qt::white);

    // Копируем исходное изображение в верхнюю часть
    QPainter painter(&newImage);
    painter.drawImage(0, 0, image);

    // Настройка для текста
    if( mPlayer.mStepIndex < mStepList.size() ) {
      QString titer = mStepList.at( mPlayer.mStepIndex ).mTiter;

      // Определяем область для текста (нижние 200 пикселей)
      QRect textRect(10, image.height() + 10, image.width() - 20, 180); // с отступами
      painter.setPen(Qt::black);

      // Настройка шрифта
      QFont textFont("Arial", 18);
      textFont.setWeight(QFont::Normal);
      painter.setFont(textFont);

      // Рисуем текст с переносом слов и выравниванием по левому краю
      painter.drawText(textRect, Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop, titer);
      }


    // Опционально: добавляем рамку вокруг текстовой области
    // painter.setPen(Qt::gray);
    // painter.drawRect(textRect.adjusted(-2, -2, 2, 2));

    painter.end();

    //Получаем указатель на данные пикселей
    const uchar* data = newImage.constBits();
    qint64 dataSize = newImage.sizeInBytes();

    // Отправляем данные в FFmpeg
    qint64 written = mFfmpeg.write(reinterpret_cast<const char*>(data), dataSize);
    if (written != dataSize)
      qWarning() << "Failed to write full frame to FFmpeg";

    if( mEventIndex >= mEventList.size() ) {
      //Stop capture
      mFfmpeg.closeWriteChannel();
      mFfmpeg.waitForFinished(3000);
      }
    }
  if( mEventIndex >= mEventList.size() ) {
    mPlayerTimer.stop();
    }
  }

void SdGuiderCapture::playStart()
  {
  if( mEventList.count() > 3 ) {
    mEventIndex = 1;
    mPlayer = mEventList.at(0);
    mCapture = false;
    }
  }




void SdGuiderCapture::captureStart()
  {
  QPixmap pixmap = mMainWindow->grab();
  // Формируем аргументы для FFmpeg
  QStringList args;
  args << "-f" << "rawvideo"
       << "-pix_fmt" << "bgra"      // подходит для QImage::Format_ARGB32 / Format_RGB32
       << "-s" << QString("%1x%2").arg(pixmap.width()).arg(pixmap.height() + 200)
       << "-r" << QString::number(20)
       << "-i" << "pipe:0"
       << "-c:v" << "libx264"
       << "-preset" << "ultrafast"
       << "-pix_fmt" << "yuv420p"
       << mVideoPath;

  // Запускаем процесс
  mFfmpeg.setProgram( "ffmpeg" );
  mFfmpeg.setArguments(args);
  mFfmpeg.setProcessChannelMode(QProcess::MergedChannels);

  mFfmpeg.start();
  if( !mFfmpeg.waitForStarted() )
    qWarning() << "Failed to start FFmpeg:" << mFfmpeg.errorString();
  else {
    qDebug() << "FFmpeg started, recording video...";
    mCapture = true;
    mPlayerTimer.start( 50 );
    }
  }

