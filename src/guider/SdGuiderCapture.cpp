/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdGuiderCapture Event capture, playback and video record
  This object is created by the main window as a single instance.
  It is used for recording macro sequences, playing them back,
  and generating video during playback.
  Record, playback, and video generation operations are performed
  for a single scene.
*/
#include "SdConfig.h"
#include "SdGuiderCapture.h"
#include "SvLib/SvJsonIO.h"
#include "objects/SdEnvir.h"

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
#include <QMessageBox>

#define RECORD_PERIOD 40
#define PLAY_PERIOD   50


//!
//! \brief SdGuiderCapture Constructor
//! \param main            Pointer to the main application window
//! \param parent          Parent QObject
//!
SdGuiderCapture::SdGuiderCapture(QWidget *main, QObject *parent) :
  QObject(parent),
  //mFile(nullptr),
  mMainWindow(main)
  {
  memset( &mEvent, 0, sizeof(SdGuiderEvent) );
  memset( &mPlayer, 0, sizeof(SdGuiderEvent) );

  QCoreApplication::instance()->installEventFilter( this );

  connect( &mRecordTimer, &QTimer::timeout, this, &SdGuiderCapture::periodicRecord );
  connect( &mPlayerTimer, &QTimer::timeout, this, &SdGuiderCapture::periodicPayer );

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



//!
//! \brief setScena   Sets a new working scene
//! \param scriptPath Path to the script
//! \param scenaIndex Index of the scene
//! \param list       Step list for the scene
//!
//! After setting, the scene can be recorded (old scene will be overwritten),
//! played back, or have a video generated from it
//!
void SdGuiderCapture::setScena(const QString &scriptPath, int scenaIndex, const SdGuiderStepList &list)
  {
  mStepList   = list;
  QString eventName("scena-%1.events");
  mEventPath = scriptPath + eventName.arg(scenaIndex);
  mVideoPath = moviePath( scriptPath, scenaIndex, false );
  mAVPath    = moviePath( scriptPath, scenaIndex, true );
  mAudioPattern = scriptPath + QString("scena-%1-stp-%3-%2.wav").arg(scenaIndex).arg( SdEnvir::languageGet() );
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




//!
//! \brief eventFilter Event filter function
//! \param watched     Object being watched
//! \param event       Event to filter
//! \return            true if event was handled, false otherwise
//!
//! Serves to intercept all program events.
//! Based on intercepted events, mEvent is set, which reflects
//! the current state of program events (mouse position, mouse buttons,
//! keyboard state)
//!
bool SdGuiderCapture::eventFilter(QObject *watched, QEvent *event)
  {
  Q_UNUSED(watched)
  QPoint mousePos = QCursor::pos( QGuiApplication::primaryScreen() ) - mMainWindow->pos();
  switch (event->type()) {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick: {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
      //QPoint c = QCursor::pos( QGuiApplication::primaryScreen() );
      if( mouseEvent->pointCount() ) {
        mEvent.mMousePosX = mousePos.x();
        mEvent.mMousePosY = mousePos.y();
        mEvent.mMouseButtons = mouseEvent->buttons();
        mEvent.mKeyModifier = mouseEvent->modifiers();
        }
      break;
      }

    case QEvent::KeyPress:
    case QEvent::KeyRelease: {
      QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
      if( keyEvent->key() == Qt::Key_F11 ) {
        //F11 pressed or released
        if( event->type() == QEvent::KeyPress ) {
          if( mRecordTimer.isActive() )
            mRecordTimer.stop();
          else
            mRecordTimer.start( RECORD_PERIOD );
          emit recordStatus( mRecordTimer.isActive() );
          }
        return true;
        }
      else if( keyEvent->key() == Qt::Key_F10 ) {
        //F10 pressed or released
        if( event->type() == QEvent::KeyPress ) {
          nextStep();
          }
        return true;
        }
      else {
        mEvent.mKeyCode = keyEvent->key();
        mEvent.mKeyChar = keyEvent->text().isEmpty() ? 0 : keyEvent->text().at(0).unicode();
        mEvent.mKeyModifier = keyEvent->modifiers();

        // Для событий клавиатуры также обновляем позицию мыши
        mEvent.mMousePosX = mousePos.x();
        mEvent.mMousePosY = mousePos.y();
        mEvent.mMouseButtons = QGuiApplication::mouseButtons();
        }
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

    // case QEvent::Enter:
    // case QEvent::Leave: {
    //   QEnterEvent *enterEvent = static_cast<QEnterEvent*>(event);
    //   if( enterEvent->pointCount() ) {
    //     mEvent.mMousePosX = enterEvent->globalPosition().x();
    //     mEvent.mMousePosY = enterEvent->globalPosition().y();
    //     mEvent.mMouseButtons = QGuiApplication::mouseButtons();
    //     mEvent.mKeyModifier = QGuiApplication::keyboardModifiers();
    //     }
    //   break;
    //   }

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



//!
//! \brief moviePath  Returns the full path and filename to the video file for the given scene
//! \param scriptPath Path to the script
//! \param scenaIndex Index of the scene
//! \return           Full path to the video file
//!
QString SdGuiderCapture::moviePath(const QString &scriptPath, int scenaIndex, bool withAudio)
  {
  if( withAudio )
    return scriptPath + QString("avScena-%1-%2.mp4").arg(scenaIndex).arg( SdEnvir::languageGet() );
  return scriptPath + QString("scena-%1-%2.mp4").arg(scenaIndex).arg( SdEnvir::languageGet() );
  }




//!
//! \brief periodicRecord Called periodically during recording
//!
//! Here the current event state is recorded into the scene's event list
//!
void SdGuiderCapture::periodicRecord()
  {
  if( mEventList.size() > 0 && mEventList.last().mStepIndex != 0 && mEvent.mStepIndex == 0 )
    mEventList.clear();
  //Limit scene size for 5min
  if( mEventList.size() < 15000 )
    mEventList.append( mEvent );
  }









//!
//! \brief periodicPayer Called periodically during playback or video generation
//!
//! Sequentially plays back events recorded in the event list.
//! During video capture, takes a snapshot of the application window,
//! adds mouse cursor with button states, adds subtitles, and sends
//! to ffmpeg for video recording
//!
void SdGuiderCapture::periodicPayer()
  {
  QPoint mainWindowPos = mMainWindow->pos();
  if( mCapture ) {
    mPlayer.inject( mEventList.at(mEventIndex++), mainWindowPos );
    mStepDuration--;
    if( ((mEventIndex >= mEventList.size()) || (mPlayer.mStepIndex != mEventList.at(mEventIndex).mStepIndex)) && mStepDuration > 0 )
      //Here we extend the video by repeating the last frame.
      mEventIndex--;
    //QPoint c = QCursor::pos( QGuiApplication::primaryScreen() );
    QCursor::setPos( QPoint(mPlayer.mMousePosX, mPlayer.mMousePosY) + mainWindowPos );

    static QImage mouse[8];

    if( mouse[0].isNull() ) {
      mouse[0] = QImage(QString(":/pic/mouse0.png"));
      mouse[1] = QImage(QString(":/pic/mouse1.png"));
      mouse[2] = QImage(QString(":/pic/mouse2.png"));
      mouse[3] = QImage(QString(":/pic/mouse3.png"));
      mouse[4] = QImage(QString(":/pic/mouse4.png"));
      mouse[5] = QImage(QString(":/pic/mouse5.png"));
      mouse[6] = QImage(QString(":/pic/mouse6.png"));
      mouse[7] = QImage(QString(":/pic/mouse7.png"));
      }

    //Capture current screen and append it to file
    QRect r = mMainWindow->frameGeometry();
    QPoint p = mMainWindow->pos();
    QSize s = mMainWindow->size();
    QPixmap pix = QGuiApplication::primaryScreen()->grabWindow( 0, p.x(), p.y(), s.width(), r.height() );
    QImage image = pix.toImage();

    // Создаем новое изображение с увеличенной высотой
    int newHeight = image.height() + 200;
    QImage newImage(image.width(), newHeight, QImage::Format_ARGB32);
    newImage.fill(Qt::white);

    // Копируем исходное изображение в верхнюю часть
    QPainter painter(&newImage);
    painter.drawImage(0, 0, image);

    int mouseIndex = 0;
    if( mPlayer.mMouseButtons & Qt::LeftButton ) mouseIndex |= 1;
    if( mPlayer.mMouseButtons & Qt::MiddleButton ) mouseIndex |= 2;
    if( mPlayer.mMouseButtons & Qt::RightButton ) mouseIndex |= 4;
    //Draw mouse
    painter.drawImage( QPoint( mPlayer.mMousePosX - 16, mPlayer.mMousePosY ), mouse[mouseIndex] );


    // Настройка для текста
    if( mPlayer.mStepIndex < mStepList.size() ) {
      QString titer = mStepList.at( mPlayer.mStepIndex ).mTitle;

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
    if( written != dataSize )
      qWarning() << "Failed to write full frame to FFmpeg";

    if( mEventIndex >= mEventList.size() ) {
      //Stop capture
      mFfmpeg.closeWriteChannel();
      mFfmpeg.waitForFinished(3000);

      //Conversion to audio mixed
      QStringList attr;
      attr << "-i" << mVideoPath;
      QString filterComplex;
      int audioCount = 0;
      int positionMs = 0;
      for( int stepIndex = 0; stepIndex < mStepList.count(); ++stepIndex ) {
        QString audioFile( mAudioPattern.arg( stepIndex ) );
        if( QFile::exists( audioFile ) ) {
          //Audio file exists
          attr << "-i" << audioFile;
          filterComplex.append( QString("[%1:a]adelay=%2|%2[a%1];").arg(++audioCount).arg(positionMs) );
          }
        positionMs += mStepList.at(stepIndex).mDuration * 1000;
        }

      //Complete filterComplex
      for( int i = 1; i <= audioCount; ++i )
        filterComplex.append( QString("[a%1]").arg(i) );
      filterComplex.append( QString("amix=inputs=%1[aout]").arg(audioCount) );

      attr << "-filter_complex" << filterComplex
           << "-map" << "0:v"
           << "-map" << "[aout]"
           << "-c:v" << "copy"
           << "-c:a" << "aac"
           << "-y"
           << mAVPath;

      qDebug() << "Audio mixing" << attr;
      QProcess ffmpeg;
      ffmpeg.start( "ffmpeg", attr );
      if( !ffmpeg.waitForStarted(5000) )
        return;

      ffmpeg.waitForFinished(35000);
      }
    }
  else {
    mPlayer.inject( mEventList.at(mEventIndex++), mainWindowPos );
    //QPoint c = QCursor::pos( QGuiApplication::primaryScreen() );
    QCursor::setPos( QPoint(mPlayer.mMousePosX, mPlayer.mMousePosY) + mainWindowPos );
    }
  if( mEventIndex >= mEventList.size() ) {
    mPlayerTimer.stop();
    QMessageBox::information( mMainWindow, tr("Info"), tr("Play completed") );
    }
  }



//!
//! \brief playStart Starts the macro sequence playback process
//!
void SdGuiderCapture::playStart()
  {
  if( mEventList.count() > 3 ) {
    mEventIndex = 1;
    mPlayer = mEventList.at(0);
    mCapture = false;
    mPlayerTimer.start( PLAY_PERIOD );
    }
  }



struct WavHeader
  {
    char  riff[4];
    int   size;
    char  wave[4];
    char  fmt[4];
    int   fmtSize;
    short audioFormat;
    short channels;
    int   sampleRate;
    int   byteRate;
    short blockAlign;
    short bitsPerSample;
    char  data[4];
    int   dataSize;
  };




//!
//! \brief captureStart Starts the macro sequence playback process with video recording
//!
void SdGuiderCapture::captureStart()
  {
  if( mEventList.count() > 3 ) {
    //Сначала посчитаем длительность видео
    int stepIndex = 0;
    int stepDuration = 0;
    for( int i = 0; i < mEventList.size(); ++i ) {
      if( mEventList.at(i).mStepIndex == stepIndex )
        stepDuration++;
      else {
        if( stepIndex < mStepList.size() ) {
          //Set fact duration of video
          mStepList[stepIndex].mDuration = stepDuration / (1000/RECORD_PERIOD);
          qDebug() << "Step " << stepIndex << " duration is " << mStepList[stepIndex].mDuration << "sec";
          }
        //Start next duration calculation
        stepDuration = 1;
        stepIndex = mEventList.at(i).mStepIndex;
        }
      }
    //Set last step duration
    mStepList.last().mDuration = stepDuration / (1000/RECORD_PERIOD);
    qDebug() << "Step " << stepIndex << " duration is " << mStepList.last().mDuration << "sec";

    //Сначала пройдем по фразам и сгенерим звуки
    QString piperBase("/home/usik/info/piper/piper/");
    QString piperProg( piperBase + "piper" );
    QString piperModel;
    if( SdEnvir::languageGet() == QString("ru") )
      piperModel = piperBase + QString("ru_RU-irina-medium.onnx");
    else
      piperModel = piperBase + QString("en_US-amy-medium.onnx");
    for( int stepIndex = 0; stepIndex < mStepList.count(); ++stepIndex ) {
      QString audioFile( mAudioPattern.arg( stepIndex ) );
      QProcess ap;
      ap.start( piperProg, {
                  "--model", piperModel,
                  "--output_file", audioFile
                });
      if( !ap.waitForStarted(5000) )
        break;

      QString text = mStepList.at(stepIndex).mTitle;
      if( SdEnvir::languageGet() == QString("ru") && text.contains("SalixEDA") )
        text = text.replace( "SalixEDA", "саликс еда" );
      ap.write( text.toUtf8() );
      ap.closeWriteChannel();

      if( !ap.waitForFinished(15000) )
        break;

      //Audio file ready
      QFile wav(audioFile);
      //We open it and read header
      if( wav.open(QIODevice::ReadOnly) ) {
        WavHeader header;
        wav.read( (char*)&header, sizeof(header) );
        //Calculation fact audio duration
        int duration = (header.dataSize + header.byteRate - 1) / header.byteRate;
        //If audio duration greater than video duration
        // than replace video duration with audio duration
        if( mStepList.at(stepIndex).mDuration < duration )
          mStepList[stepIndex].mDuration = duration;
        qDebug() << "Audio generated " << audioFile << "with duration " << duration << "sec" ;
        }
      }


    mEventIndex = 1;
    mPlayer = mEventList.at(0);
    mStepDuration = stepEventDuration(0);
    QRect r = mMainWindow->frameGeometry();
    QSize s = mMainWindow->size();
    // Формируем аргументы для FFmpeg
    QStringList args;
    args << "-f" << "rawvideo"
         << "-pix_fmt" << "bgra"      // подходит для QImage::Format_ARGB32 / Format_RGB32
         << "-s" << QString("%1x%2").arg(s.width()).arg(r.height() + 200)
         << "-r" << QString::number(1000/RECORD_PERIOD)
         << "-i" << "pipe:0"
         << "-c:v" << "libx264"
         << "-preset" << "slow"
         << "-crf" << "20"
         << "-pix_fmt" << "yuv420p"
         << "-y"
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
      mPlayerTimer.start( PLAY_PERIOD );
      }
    }
  }





//!
//! \brief nextStep Advances to the next step within the scene
//!
void SdGuiderCapture::nextStep()
  {
  mEvent.mStepIndex++;
  if( mEvent.mStepIndex >= mStepList.count() ) {
    mRecordTimer.stop();
    emit recordStatus( false );
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





//!
//! \brief stepEventDuration Returns step duration in events (how mutch events within step)
//! \param stepIndex         Step index of calculated step
//! \return                  Step duration in events
//!
int SdGuiderCapture::stepEventDuration(int stepIndex)
  {
  if( stepIndex < mStepList.size() )
    return mStepList.at(stepIndex).mDuration * 1000/RECORD_PERIOD;
  return 0;
  }





