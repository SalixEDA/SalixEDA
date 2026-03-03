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
#ifndef SDGUIDERPLAYER_H
#define SDGUIDERPLAYER_H

#include <QWidget>
#include <QTimer>
#include <QImage>

class SdGuiderPlayer : public QWidget
  {
    Q_OBJECT

    QTimer  mTimer;     //Timer for interval event generating
    qint32  mTime;      //Current play moment
  public:
    explicit SdGuiderPlayer(QWidget *parent = nullptr);


  signals:

  public slots:
    void onTime();

    void play( const QString &fname );

    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;
  };

#endif // SDGUIDERPLAYER_H
