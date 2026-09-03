/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  The converter performs mirroring across the segment. Used for display selected objects in appropriate place
*/
#ifndef SDCONVERTERGRAPHMIRROR_H
#define SDCONVERTERGRAPHMIRROR_H

#include "SdConverter.h"
#include "SdPoint.h"

class SdConverterGraphMirror : public SdConverter
  {
    SdPoint mFrom; //!< First point of segment
    SdPoint mTo;   //!< Second point of segment
  public:
    SdConverterGraphMirror( SdPoint from, SdPoint to );

    // SdConverter interface
  public:
    virtual QTransform getMatrix() override;
  };

#endif // SDCONVERTERGRAPHMIRROR_H
