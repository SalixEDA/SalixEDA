/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Dialog for display guider playback
*/
#ifndef SDDGUIDERPLAYER_H
#define SDDGUIDERPLAYER_H

#include <QDialog>
#include <QLabel>
#include <QTimer>

class QMediaPlayer;
class QVideoWidget;

class SdDGuiderPlayer : public QDialog
  {
    Q_OBJECT

    QMediaPlayer *mPlayer; //!< Media player
    QVideoWidget *mVideo;  //!< Video widget
  public:
    SdDGuiderPlayer( const QString fname, QWidget *parent = nullptr );

    static QString guiderPath();

    static bool    guiderExist( const QString fname );
  };

#endif // SDDGUIDERPLAYER_H
