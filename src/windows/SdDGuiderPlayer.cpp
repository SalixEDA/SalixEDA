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
#include "SdConfig.h"
#include "SdDGuiderPlayer.h"
#include "SdWHelp.h"
#include "objects/SdEnvir.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QDebug>



SdDGuiderPlayer::SdDGuiderPlayer(const QString fname, QWidget *parent) :
  QDialog( parent )
  {
  //Setup titer and speach language
  mPlayer = new QMediaPlayer(this);
  mVideo  = new QVideoWidget(this);

  QString fileName = guiderPath() + fname + QString("-%1.mp4").arg( SdEnvir::languageGet() );
  if( !QFile::exists(fileName) )
    fileName = guiderPath() + fname + QString("-en.mp4");

  qDebug() << fileName;

  mPlayer->setSource( QUrl::fromLocalFile(fileName) );
  mPlayer->setVideoOutput( mVideo );

  resize( 1600, 950 );

  //Construct help window
  setWindowTitle( tr("Guider") );
  QVBoxLayout *box = new QVBoxLayout();
  QToolBar *bar = new QToolBar();
  box->addWidget( bar );

  QToolButton *tool = new QToolButton();
  tool->setToolTip( tr("Close") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderEject.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, this, &SdDGuiderPlayer::close );

  tool = new QToolButton();
  tool->setToolTip( tr("Restart") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderBackward.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, this, [this]() {
    mPlayer->stop();
    mPlayer->setPosition(0);
    mPlayer->play();
    } );

  tool = new QToolButton();
  tool->setToolTip( tr("Start play") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderStart.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, mPlayer, &QMediaPlayer::play );
  connect( mPlayer, &QMediaPlayer::playingChanged, tool, &QToolButton::setDisabled );

  tool = new QToolButton();
  tool->setToolTip( tr("Pause") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderPause.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, mPlayer, &QMediaPlayer::pause );
  connect( mPlayer, &QMediaPlayer::playingChanged, tool, &QToolButton::setEnabled );

  tool = new QToolButton();
  tool->setToolTip( tr("Stop") );
  tool->setIcon( QIcon(QStringLiteral(":/pic/guiderStop.png")) );
  bar->addWidget( tool );
  connect( tool, &QToolButton::clicked, mPlayer, &QMediaPlayer::stop );
  connect( mPlayer, &QMediaPlayer::playingChanged, tool, &QToolButton::setEnabled );


  box->addWidget( mVideo );
  setLayout( box );
  mPlayer->play();
  }




QString SdDGuiderPlayer::guiderPath()
  {
  return QCoreApplication::applicationDirPath() + QString("/guide/");
  }






bool SdDGuiderPlayer::guiderExist(const QString fname)
  {
  return QFile::exists( guiderPath() + fname + QString("-%1.mp4").arg( SdEnvir::languageGet() ) );
  }

