/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Plate net container. Contains net segments with appropriate traced objects.
  Used for segments accumulation.
*/
#ifndef SDPLATENETCONTAINER_H
#define SDPLATENETCONTAINER_H

#include "SdPoint.h"
#include "SdPvStratum.h"
#include "SdGraphTraced.h"

class SdPlateNetContainer
  {
  public:
    SdPlateNetContainer();
    virtual ~SdPlateNetContainer();

    virtual void addNetSegment( SdGraphTraced *traced, const QString netName, SdPvStratum s, SdPoint p1, SdPoint p2 ) = 0;
  };

#endif // SDPLATENETCONTAINER_H
