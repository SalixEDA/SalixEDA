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


SdModeCNetPinsList::SdModeCNetPinsList(SdWEditorGraph *editor, SdProjectItem *obj)
  : SdMode( editor, obj )
  {

  }


int SdModeCNetPinsList::getPropBarId() const
  {
  return PB_TEXT;
  }




void SdModeCNetPinsList::enterPoint(SdPoint p)
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

  if( pgraph == nullptr ) {
    //Edit new net list
    SdDNetPinsList netPinsListDlg( mObject, mEditor );

    if( netPinsListDlg.exec() ) {
      //Insert new net pins list
      addPic( pgraph = new SdGraphNetPinsList( *(sdGlobalProp->propText(mObject->getClass())), p ), QStringLiteral("Append new net pins list")  );
      pgraph->pinListSet( netPinsListDlg.netName(), netPinsListDlg.pinList(), mUndo );
      }
    }
  else {
    //Edit existing net list
    SdDNetPinsList netPinsListDlg( mObject, mEditor );

    netPinsListDlg.setup( pgraph->netName(), pgraph->pinList() );
    if( netPinsListDlg.exec() ) {
      //Insert new net pins list
      mUndo->begin( QStringLiteral("Edit net pins list"), mObject, false );
      pgraph->pinListSet( netPinsListDlg.netName(), netPinsListDlg.pinList(), mUndo );
      }
    }
  }




void SdModeCNetPinsList::cancelPoint(SdPoint)
  {
  cancelMode();
  }




void SdModeCNetPinsList::movePoint(SdPoint)
  {
  }




QString SdModeCNetPinsList::getStepHelp() const
  {
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
  return CUR_SCRIPT;
  }




int SdModeCNetPinsList::getIndex() const
  {
  return MD_NET_LIST;
  }
