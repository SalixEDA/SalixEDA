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
#include "SdModeCLinear.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdWCommand.h"
#include "windows/SdPropBarLinear.h"

SdModeCLinear::SdModeCLinear(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {
  }


int SdModeCLinear::getPropBarId() const
  {
  return PB_LINEAR;
  }



void SdModeCLinear::propGetFromBar()
  {
  SdPropBarLinear *bar = dynamic_cast<SdPropBarLinear*>( SdWCommand::mBarTable[PB_LINEAR] );
  if( bar ) {
    bar->getPropLine( sdGlobalProp->propLine( mObject->getClass() ), &(sdGlobalProp->mLineEnterType) );
    mEditor->setFocus();
    update();
    }
  }



void SdModeCLinear::propSetToBar()
  {
  SdPropBarLinear *bar = dynamic_cast<SdPropBarLinear*>( SdWCommand::mBarTable[PB_LINEAR] );
  if( bar )
    bar->setPropLine( sdGlobalProp->propLine( mObject->getClass() ), mEditor->getPPM(), sdGlobalProp->mLineEnterType );
  }
