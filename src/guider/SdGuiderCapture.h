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
#ifndef SDGUIDERCAPTURE_H
#define SDGUIDERCAPTURE_H

#include "SdGuiderEvent.h"
#include "SdGuiderStep.h"

#include <QObject>
#include <QTimer>
#include <QProcess>

class SdGuiderCapture : public QObject
  {
    Q_OBJECT
    SdGuiderEvent     mEvent;       //!< Current state of mouse and keyboard events
    SdGuiderEvent     mPlayer;
    SdGuiderEventList mEventList;   //!< Event list for scenaIndex
    QString           mEventPath;   //!< Full path to event list for current scenaIndex
    QString           mVideoPath;   //!< Full path to video file
    SdGuiderStepList  mStepList;    //!< Current scena step list
    QTimer            mRecordTimer; //!< Periodic generator for record events
    QTimer            mPlayerTimer; //!< Periodic generator for play events and capture
    QWidget          *mMainWindow;
    QProcess          mFfmpeg;
    int               mEventIndex;
    bool              mCapture;
  public:
    explicit SdGuiderCapture(QWidget *main, QObject *parent = nullptr);

    void setScena( const QString &scriptPath, int scenaIndex, const SdGuiderStepList &list );

    bool isCapture() const { return mPlayerTimer.isActive(); }

    bool isRecord() const { return mRecordTimer.isActive(); }

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

  signals:
    void stepIndexChanged( int stepIndex );

  public slots:
    void periodicRecord();

    void recordPauseResume();

    void nextStep();


    void periodicPayer();

    void playStart();

    void captureStart();
  };

#endif // SDGUIDERCAPTURE_H
