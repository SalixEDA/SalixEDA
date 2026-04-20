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
  We store mouse coordinates relative to the application window.
  During recording, after obtaining global coordinates, we convert
  them to coordinates relative to the application window, and during
  playback, we restore global coordinates by shifting them by the
  window position value.
*/
#ifndef SDGUIDERCAPTURE_H
#define SDGUIDERCAPTURE_H

#include "SdGuiderEvent.h"
#include "SdGuiderStep.h"

#include <QObject>
#include <QTimer>
#include <QProcess>

//!
//! \brief SdGuiderCapture Event capture, playback and video record
//!
//! This object is created by the main window as a single instance.
//! It is used for recording macro sequences, playing them back,
//! and generating video during playback.
//! Record, playback, and video generation operations are performed
//! for a single scene.
//!
class SdGuiderCapture : public QObject
  {
    Q_OBJECT
    SdGuiderEvent     mEvent;         //!< Current state of mouse and keyboard events
    SdGuiderEvent     mPlayer;        //!< Player event state
    SdGuiderEventList mEventList;     //!< Event list for current scenaIndex
    QString           mScriptPath;    //!< Path to script directory
    QString           mEventPath;     //!< Full path to event list for current scenaIndex
    QString           mVideoPath;     //!< Full path to video file
    QString           mAVPath;        //!< Full path to video with audio
    QString           mAudioPattern;  //!< Pattern for audio files
    QString           mShotPath;      //!< Pattern for screen shot files
    SdGuiderStepList  mStepList;      //!< Step list for current scene
    QTimer            mRecordTimer;   //!< Periodic timer for generating record events
    QTimer            mPlayerTimer;   //!< Periodic timer for generating playback events and capture
    QWidget          *mMainWindow;    //!< Main application window
    QProcess          mFfmpeg;        //!< FFmpeg process for video encoding
    int               mEventIndex;    //!< Current event index during playback
    int               mStepDuration;  //!< Step duration in events
    int               mShotStepIndex; //!< Shot step index
    bool              mCapture;       //!< Flag indicating whether video capture is active
    bool              mShoting;       //!< Flag indicating whether screen shot capture is active
  public:
    //!
    //! \brief SdGuiderCapture Constructor
    //! \param main            Pointer to the main application window
    //! \param parent          Parent QObject
    //!
    explicit SdGuiderCapture(QWidget *main, QObject *parent = nullptr);

    //!
    //! \brief setScena   Sets a new working scene
    //! \param scriptPath Path to the script
    //! \param scenaIndex Index of the scene
    //! \param list       Step list for the scene
    //!
    //! After setting, the scene can be recorded (old scene will be overwritten),
    //! played back, or have a video generated from it
    //!
    void setScena( const QString &scriptPath, int scenaIndex, const SdGuiderStepList &list );

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
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    //!
    //! \brief moviePath  Returns the full path and filename to the video file for the given scene
    //! \param scriptPath Path to the script
    //! \param scenaIndex Index of the scene
    //! \return           Full path to the video file
    //!
    static QString moviePath( const QString &scriptPath, int scenaIndex, bool withAudio );
  signals:
    //!
    //! \brief stepIndexChanged Emitted when the current step index changes
    //! \param stepIndex        New step index
    //!
    void stepIndexChanged( int stepIndex );

    //!
    //! \brief recordStatus Emitted when recording status changes
    //! \param isRecord     true if recording is active, false otherwise
    //!
    void recordStatus( bool isRecord );
  public slots:
    //!
    //! \brief periodicRecord Called periodically during recording
    //!
    //! Here the current event state is recorded into the scene's event list
    //!
    void periodicRecord();

    //!
    //! \brief periodicPayer Called periodically during playback or video generation
    //!
    //! Sequentially plays back events recorded in the event list.
    //! During video capture, takes a snapshot of the application window,
    //! adds mouse cursor with button states, adds subtitles, and sends
    //! to ffmpeg for video recording
    //!
    void periodicPayer();

    //!
    //! \brief playStart Starts the macro sequence playback process
    //!
    void playStart();

    //!
    //! \brief captureStart Starts the macro sequence playback process with video recording
    //!
    void captureStart();

    //!
    //! \brief shotingStart Starts the macro sequence playback process with screen shots recording
    //!
    void shotingStart();


    //!
    //! \brief screenShot Make screen shot and save it as png file into guide directory
    //!
    //void screenShot();

  private:
    //!
    //! \brief nextStep Advances to the next step within the scene
    //!
    void nextStep();

    //!
    //! \brief stepEventDuration Returns step duration in events (how mutch events within step)
    //! \param stepIndex         Step index of calculated step
    //! \return                  Step duration in events
    //!
    int  stepEventDuration( int stepIndex );
  };
#endif // SDGUIDERCAPTURE_H
