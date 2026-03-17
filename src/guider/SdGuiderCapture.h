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

#include <QObject>
#include <QTimer>

class SdGuiderCapture : public QObject
  {
    Q_OBJECT

    QTimer        mTimer;
    QWidget      *mMainWindow;
    int           mTime;
  public:
    explicit SdGuiderCapture(QWidget *main, QObject *parent = nullptr);

    void setScena( const QString &scriptPath, int scenaIndex );

    bool isCapture() const { return mFile != nullptr; }

    bool isPaused() const { return !mTimer.isActive(); }

  signals:

  public slots:
    //Init capture process
    void captureInit();

    //
    void captureStop();

    //Pause capture process
    void capturePause();

    //Resume capture process after pause
    void captureResume();
  };

#endif // SDGUIDERCAPTURE_H
