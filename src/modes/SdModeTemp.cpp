/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base mode for temporary modes.
*/
#include "SdModeTemp.h"

SdModeTemp::SdModeTemp( SdWEditorGraph *editor, SdProjectItem *obj ) :
  SdMode( editor, obj ),
  mMainMode(0)
  {

  }




void SdModeTemp::drawStatic(SdContext *ctx)
  {
  //Draw full picture
  mObject->draw( ctx );
  }
