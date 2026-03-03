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

#include "SdLibraryIndicator.h"

#include <QToolButton>

SdLibraryIndicator::SdLibraryIndicator()
  : mMsgNone("<font color='black'>✓&nbsp;</font>")
  , mMsgOk("<font color='green'>✓&nbsp;</font>")
  , mMsgTransfer( "<font color='blue'>⇄&nbsp;</font>" )
  , mMsgError( "<font color='red'>⚠&nbsp;</font>")
  {
  }



void SdLibraryIndicator::addToStatusBar(QStatusBar *sbar)
  {
  QToolButton *but = new QToolButton();
  but->setDefaultAction( mGlobalStorage = new QAction( QIcon(QString(":/pic/iconGlobalStorage.png")), QString{} ) );
  sbar->addWidget( but );
  sbar->addWidget( mGlobalStorageIndicator = new QLabel(mMsgOk) );
  mGlobalStorage->setToolTip( tr("Prepare remote global storage server connection") );

  but = new QToolButton();
  but->setDefaultAction( mPrivateCloud = new QAction( QIcon(QString(":/pic/iconPrivateCloud.png")), QString{} ) );
  sbar->addWidget( but );
  sbar->addWidget( mPrivateCloudIndicator = new QLabel(mMsgOk) );
  mPrivateCloud->setToolTip( tr("Prepare remote private cloud server connection") );

  but = new QToolButton();
  but->setDefaultAction( mLocalLibrary = new QAction( QIcon(QString(":/pic/iconLocalLibrary.png")), QString{} ) );
  sbar->addWidget( but );
  sbar->addWidget( mLocalLibraryIndicator = new QLabel(mMsgOk) );
  mLocalLibrary->setToolTip( tr("Prepare local library scan") );
  }



void SdLibraryIndicator::setGlobalStorage(int transferOut, int transferIn, int stat, const QString errorMsg)
  {
  switch( stat ) {
    case SdLisNoTransfer :
      mGlobalStorageIndicator->setText( mMsgOk );
      break;

    case SdLisTransfer :
      mGlobalStorageIndicator->setText( mMsgTransfer );
      break;

    case SdLisError :
      mGlobalStorageIndicator->setText( mMsgError );
      mGlobalStorage->setToolTip( tr("<b>🌐 Global storage</b><br>"
                                    "Error: %1").arg(errorMsg) );
      break;
    }
  if( stat != SdLisError )
    mGlobalStorage->setToolTip( tr("<b>🌐 Global storage</b><br>"
                                   "Transfer in: %1<br>"
                                   "Transfer out: %2").arg(transferIn).arg(transferOut) );
  }



void SdLibraryIndicator::setPrivateCloud(int transferOut, int transferIn, int stat, const QString errorMsg)
  {
  switch( stat ) {
    case SdLisNoTransfer :
      mPrivateCloudIndicator->setText( mMsgOk );
      break;

    case SdLisTransfer :
      mPrivateCloudIndicator->setText( mMsgTransfer );
      break;

    case SdLisError :
      mPrivateCloudIndicator->setText( mMsgError );
      mPrivateCloud->setToolTip( tr("<b>☁️ Private cloud</b><br>"
                                    "Error: %1").arg(errorMsg) );
      break;
    }
  if( stat != SdLisError )
    mPrivateCloud->setToolTip( tr("<b>☁️ Private cloud</b><br>"
                                  "Transfer in: %1<br>"
                                  "Transfer out: %2").arg(transferIn).arg(transferOut) );
  }



void SdLibraryIndicator::setLocalLibrary(int count, int appended, int removed, int updated, int stat, const QString errorMsg)
  {
  switch( stat ) {
    case SdLisNoTransfer :
      mLocalLibraryIndicator->setText( mMsgOk );
      break;

    case SdLisTransfer :
      mLocalLibraryIndicator->setText( mMsgTransfer );
      break;

    case SdLisError :
      mLocalLibraryIndicator->setText( mMsgError );
      mLocalLibrary->setToolTip( tr("<b>📚 Local library</b><br>"
                                    "Error: %1").arg(errorMsg) );
      break;
    }
  if( stat != SdLisError )
    mLocalLibrary->setToolTip( tr("<b>📚 Local library</b><br>"
                                  "Objects in library: %1<br>"
                                  "Appended: %2, Removed: %3<br>"
                                  "Updated: %4").arg(count).arg(appended).arg(removed).arg(updated) );
  }
