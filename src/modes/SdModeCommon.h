/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Common mode for some graph modes
*/

#ifndef SDMODECOMMON_H
#define SDMODECOMMON_H

#include "SdMode.h"

class SdModeCommon : public SdMode
  {
  protected:
    SdPoint mPrev; //Предыдущая точка (при перемещении)
  public:
    SdModeCommon( SdWEditorGraph *editor, SdProjectItem *obj );

  };

#endif // SDMODECOMMON_H
