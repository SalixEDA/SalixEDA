/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Listing of masters for export and import objects.

  Show list masters which can export or import object.
*/
#include "SdPMasterList.h"


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include <QSettings>
#include <QMessageBox>

SdPMasterList::SdPMasterList( const QString atitle, const QString asubTitle, QWidget *parent) :
  QWizardPage(parent)
  {
  setTitle( atitle );
  setSubTitle( asubTitle );

  QHBoxLayout *hlay = new QHBoxLayout();
  QVBoxLayout *vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Select master type") ) );
  mMasterType = new QListWidget();
  vlay->addWidget( mMasterType );
  hlay->addLayout( vlay );

  vlay = new QVBoxLayout();
  vlay->addWidget( mImage = new QLabel() );
  hlay->addLayout( vlay );

  vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Description")) );
  vlay->addWidget( mDescription = new QTextEdit() );
  mDescription->setReadOnly(true);
  hlay->addLayout( vlay );

  setLayout( hlay );

  connect( mMasterType, &QListWidget::currentRowChanged, this, [this] ( int row ) {
    if( row >= 0 ) {
      mDescription->setText( mInfoList.at(row).mMasterDescription );
      mImage->setPixmap( QPixmap( mInfoList.at(row).mImageSrc )  );
      QSettings s;
      s.setValue( title(), row );
      }
    } );

  // Assign unique names for the interactive help system
  mMasterType->setObjectName("SdPMasterList.mMasterType");
  mImage->setObjectName("SdPMasterList.mImage");
  mDescription->setObjectName("SdPMasterList.mDescription");
  }




void SdPMasterList::addMaster(const QString name, const QString descr, int stp, const QString img)
  {
  //Fill info struct and append it to info list
  SdMasterInfo info;
  info.mImageSrc          = img;
  info.mMasterDescription = descr;
  info.mMasterName        = name;
  info.mMasterStep        = stp;
  mInfoList.append( info );
  }





void SdPMasterList::initializePage()
  {
  //Fill master list
  for( int i = 0; i < mInfoList.count(); i++ )
    mMasterType->addItem( mInfoList.at(i).mMasterName );

  QSettings s;
  mMasterType->setCurrentRow( s.value( title(), 0 ).toInt() );
  }




bool SdPMasterList::validatePage()
  {
  return mMasterType->currentRow() >= 0;
  }



int SdPMasterList::nextId() const
  {
  return mInfoList.at( mMasterType->currentRow() ).mMasterStep;
  }
