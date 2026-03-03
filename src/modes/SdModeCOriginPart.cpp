/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for part pin origin
*/
#include "SdModeCOriginPart.h"

SdModeCOriginPart::SdModeCOriginPart( SdWEditorGraph *editor, SdProjectItem *obj, int osize ) :
  SdModeCOrigin( editor, obj, osize )
  {
  mModeIndex = MD_PART_ORIGIN;
  }
