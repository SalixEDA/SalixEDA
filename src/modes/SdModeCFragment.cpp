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
#include "SdModeCFragment.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdPulsar.h"
#include "objects/SdConverterOffset.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdDGetObject.h"

#include <QMessageBox>

SdModeCFragment::SdModeCFragment(SdWEditorGraph *editor, SdProjectItem *obj, SdProject *project, const QString &objectName) :
  SdModeCommon( editor, obj ),
  mPastePrj(project),
  mObjectName(objectName)
  {

  }




SdModeCFragment::~SdModeCFragment()
  {
  clear();
  }





void SdModeCFragment::activate()
  {
  SdModeCommon::activate();
  if( mObjectName.isEmpty() )
    getFragment();
  else
    {
    mPaste.clear();
    SdPItemSheet *sheet = mPastePrj->getSheet( mObjectName );

    if( sheet == nullptr ) {
      QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("No sheets to insert. Try another fragment.") );
      cancelMode();
      return;
      }

    //Select all objects in sheet
    sheet->forEach( dctAll, [this] (SdObject *obj) -> bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr )
        graph->select( &mPaste );
      return true;
      });


    if( !mPaste.count() ) {
      QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("No objects to insert. Source sheet is empty. Select another fragment.") );
      cancelMode();
      }
    }
  }




void SdModeCFragment::drawDynamic(SdContext *ctx)
  {
  //Draw all copy in current point
  SdConverterOffset offset( mPrevMove.sub(mFirst) );
  ctx->setConverter( &offset );
  mPaste.draw( ctx );
  }




int SdModeCFragment::getPropBarId() const
  {
  return PB_DEFAULT;
  }




void SdModeCFragment::enterPoint(SdPoint point)
  {
  if( mPaste.count() ) {
    setDirty();
    mUndo->begin( QObject::tr("Insert fragment"), mObject, false );
    //Insert copy of pasted elements into object without selection them
    mObject->insertObjects( point.sub( mFirst ), &mPaste, mUndo, nullptr, false );
    }
  if( !mObjectName.isEmpty() )
    cancelMode();
  }




void SdModeCFragment::cancelPoint(SdPoint)
  {
  if( mObjectName.isEmpty() )
    getFragment();
  else
    cancelMode();
  }




void SdModeCFragment::movePoint(SdPoint p)
  {
  mPrevMove = p;
  update();
  }




QString SdModeCFragment::getStepHelp() const
  {
  return QObject::tr("Enter fragment place point");
  }




QString SdModeCFragment::getModeThema() const
  {
  return QString( MODE_HELP "ModeCFragment.htm" );
  }




QString SdModeCFragment::getStepThema() const
  {
  return getModeThema();
  }




int SdModeCFragment::getCursor() const
  {
  return CUR_PLACE;
  }




int SdModeCFragment::getIndex() const
  {
  return MD_FRAGMENT;
  }





void SdModeCFragment::getFragment()
  {
  while(1) {
    clear();
    QString sheetName;
    mPastePrj = SdDGetObject::getProject( sheetName, QObject::tr("Select fragment to insert"), mEditor );
    if( mPastePrj == nullptr ) {
      cancelMode();
      return;
      }

    SdPItemSheet *sheet = mPastePrj->getSheet( sheetName );

    if( sheet == nullptr ) {
      QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("No sheets to insert. Try another fragment.") );
      continue;
      }

    //Select all objects in sheet
    sheet->forEach( dctAll, [this] (SdObject *obj) -> bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr )
        graph->select( &mPaste );
      return true;
      });


    if( !mPaste.count() ) {
      QMessageBox::warning( mEditor, QObject::tr("Warning!"), QObject::tr("No objects to insert. Source sheet is empty. Select another fragment.") );
      continue;
      }

    break;
    }
  update();
  }



void SdModeCFragment::clear()
  {
  mPaste.clear();
  if( mPastePrj ) {
    delete mPastePrj;
    mPastePrj = nullptr;
    }
  }
