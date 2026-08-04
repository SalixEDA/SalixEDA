/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Some common params:
    - interface language
*/
#include "SdDOptionsPageCommon.h"
#include "SdLanguage.h"
#include "SdConfig.h"
#include "objects/SdEnvir.h"
#include "SvLib/SvJsonIO.h"

#include <QSettings>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QJsonObject>
#include <QFile>
#include <QLineEdit>


SdDOptionsPageCommon::SdDOptionsPageCommon(QWidget *parent) :
  QWidget(parent)
  {

  setWindowTitle( tr("Paths") );

  QGridLayout *grid = new QGridLayout();

  grid->addWidget( new QLabel(tr("Interface language:")), 0, 0 );
  grid->addWidget( mLanguage = new QComboBox(), 0, 1 );


  QString defLang = SdEnvir::languageGet();
  QString defLangTitle;

  //Get supported language list and fill language table
  SdLanguage::SdLanguageList list = SdLanguage::languageList();
  for( const SdLanguage &lang : list ) {
    //Insert language title to combo box
    mLanguage->addItem( lang.mTitle, lang.mId );
    if( defLang == lang.mId )
      defLangTitle = lang.mTitle;
    }

  //Set current language
  if( defLangTitle.isEmpty() )
    defLangTitle = list.at(0).mTitle;
  mLanguage->setCurrentText( defLangTitle );


  //Open update.cnf
  mUpdatePeriod = nullptr;
  QString updatePath( QApplication::applicationDirPath() + QString("/update.cnf") );
  if( QFile::exists( updatePath ) ) {
    QFile updateFile(updatePath);
    if( updateFile.open(QIODevice::ReadOnly) ) {
      mUpdateConfig = svJsonObjectFromByteArray( updateFile.readAll() );
      updateFile.close();

      grid->addWidget( new QLabel(tr("Update check period, days:")), 1, 0 );
      mUpdatePeriod = new QSpinBox();
      grid->addWidget( mUpdatePeriod, 1, 1 );
      mUpdatePeriod->setRange(1,3650);
      mUpdatePeriod->setValue( mUpdateConfig.value("check_period_days").toInt() );

      grid->addWidget( new QLabel(tr("Last update checked:")), 2, 0 );
      QLineEdit *edit = new QLineEdit();
      edit->setReadOnly(true);
      edit->setText( mUpdateConfig.value("last_check").toString() );
      grid->addWidget( edit, 2, 1 );
      }
    }


  setLayout( grid );

  // Assign unique names for the interactive help system
  mLanguage->setObjectName( "SdDOptionsPageCommon.mLanguage" );
  if( mUpdatePeriod != nullptr )
    mUpdatePeriod->setObjectName( "SdDOptionsPageCommon.mUpdatePeriod" );
  }



void SdDOptionsPageCommon::accept()
  {
  QString lang = mLanguage->currentData().toString();
  QSettings s;
  s.setValue( SDK_LANGUAGE, lang );

  if( mUpdatePeriod != nullptr ) {
    mUpdateConfig.insert( "check_period_days", mUpdatePeriod->value() );
    QString updatePath( QApplication::applicationDirPath() + QString("/update.cnf") );
    QFile updateFile(updatePath);
    if( updateFile.open(QIODevice::WriteOnly) ) {
      updateFile.write( svJsonObjectToByteArray( mUpdateConfig ) );
      updateFile.close();
      }
    }
  }

