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
#ifndef SDMODECORIGINPLATE_H
#define SDMODECORIGINPLATE_H

#include "SdModeCOrigin.h"


class SdModeCOriginPlate : public SdModeCOrigin
  {
  public:
    SdModeCOriginPlate( SdWEditorGraph *editor, SdProjectItem *obj, int osize );
  };

#endif // SDMODECORIGINPLATE_H
