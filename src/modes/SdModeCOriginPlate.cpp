/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for plate origin
*/
#include "SdModeCOriginPlate.h"

SdModeCOriginPlate::SdModeCOriginPlate(SdWEditorGraph *editor, SdProjectItem *obj, int osize) :
  SdModeCOrigin( editor, obj, osize )
  {
  mModeIndex = MD_PLATE_ORIGIN;
  }
