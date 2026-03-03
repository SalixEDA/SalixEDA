/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Converter for simple offset. Used for display copy of selected objects in appropriate place
*/
#ifndef SDCONVERTEROFFSET_H
#define SDCONVERTEROFFSET_H

#include "SdConverter.h"
#include "SdPoint.h"

class SdConverterOffset : public SdConverter
  {
    SdPoint mOffset;
  public:
    SdConverterOffset( SdPoint offset );

    // SdConverter interface
  public:
    virtual QTransform getMatrix() override;
  };

#endif // SDCONVERTEROFFSET_H
