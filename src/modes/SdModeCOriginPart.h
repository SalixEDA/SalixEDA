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
#ifndef SDMODECORIGINPART_H
#define SDMODECORIGINPART_H

#include "SdModeCOrigin.h"

class SdModeCOriginPart : public SdModeCOrigin
  {
  public:
    SdModeCOriginPart( SdWEditorGraph *editor, SdProjectItem *obj, int osize );
  };

#endif // SDMODECORIGINPART_H
