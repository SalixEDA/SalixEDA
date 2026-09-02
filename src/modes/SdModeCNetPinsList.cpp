/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for insert and edit net pins list
*/
#include "SdModeCNetPinsList.h"
#include "objects/SdGraphNetPinsList.h"
#include "objects/SdProp.h"
#include "windows/SdDNetPinsList.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdWCommand.h"
#include "windows/SdPropBarTextual.h"


SdModeCNetPinsList::SdModeCNetPinsList(SdWEditorGraph *editor, SdProjectItem *obj)
  : SdMode( editor, obj )
  , mOverNetPinsList(nullptr)
  {

  }


int SdModeCNetPinsList::getPropBarId() const
  {
  return SdPropBarTextual::mBarId;
  }




void SdModeCNetPinsList::propGetFromBar()
  {
  if( SdPropBarTextualPtr tbar{} ) {
    tbar->getPropText( sdGlobalProp->propText( mObject->getClass() ) );
    mEditor->setFocus();
    update();
    }
  }




void SdModeCNetPinsList::propSetToBar()
  {
  if( SdPropBarTextualPtr tbar{} ) {
    tbar->setPropText( sdGlobalProp->propText( mObject->getClass() ), mEditor->getPPM() );
    }
  }





void SdModeCNetPinsList::enterPoint(SdPoint p)
  {

  if( mOverNetPinsList == nullptr ) {
    //Edit new net list
    SdDNetPinsList netPinsListDlg( mObject, mEditor );

    if( netPinsListDlg.exec() ) {
      //Insert new net pins list
      addPic( mOverNetPinsList = new SdGraphNetPinsList( *(sdGlobalProp->propText(mObject->getClass())), p ), QStringLiteral("Append new net pins list")  );
      mOverNetPinsList->pinListSet( netPinsListDlg.netName(), netPinsListDlg.pinList(), mUndo );
      }
    }
  else {
    //Edit existing net list
    SdDNetPinsList netPinsListDlg( mObject, mEditor );

    netPinsListDlg.setup( mOverNetPinsList->netName(), mOverNetPinsList->pinList() );
    if( netPinsListDlg.exec() ) {
      //Insert new net pins list
      mUndo->begin( QStringLiteral("Edit net pins list"), mObject, false );
      mOverNetPinsList->pinListSet( netPinsListDlg.netName(), netPinsListDlg.pinList(), mUndo );
      update();
      }
    }

  mOverNetPinsList = nullptr;
  setStepStatusMessage();
  setStepCursor();
  }




void SdModeCNetPinsList::cancelPoint(SdPoint)
  {
  cancelMode();
  }




void SdModeCNetPinsList::movePoint(SdPoint p)
  {
  //Find object behind enter point
  SdGraphNetPinsList *pgraph = nullptr;
  mObject->forEach( dctText, [&pgraph,p]( SdObject *obj ) {
    SdPtr<SdGraphNetPinsList> graph(obj);
    if( graph.isValid() && graph->behindCursor(p) ) {
      pgraph = graph.ptr();
      return false;
      }
    return true;
    });

  bool changed = (pgraph == nullptr) != (mOverNetPinsList == nullptr);
  mOverNetPinsList = pgraph;

  if( changed ) {
    setStepStatusMessage();
    setStepCursor();
    }
  }




QString SdModeCNetPinsList::getStepHelp() const
  {
  if( mOverNetPinsList != nullptr )
    return QObject::tr("Enter point to edit net pins list");
  return QObject::tr("Enter point to insert or edit net pins list");
  }




QString SdModeCNetPinsList::getModeThema() const
  {
  return QString( MODE_HELP "ModeCNetPinsList.htm" );
  }




QString SdModeCNetPinsList::getStepThema() const
  {
  return getModeThema();
  }




int SdModeCNetPinsList::getCursor() const
  {
  return mOverNetPinsList != nullptr ? CUR_NET_LIST_ED : CUR_NET_LIST;
  }




int SdModeCNetPinsList::getIndex() const
  {
  return MD_NET_LIST;
  }


