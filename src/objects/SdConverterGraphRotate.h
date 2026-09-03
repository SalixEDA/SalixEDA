/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Converter for simple rotate graphics. Used for display selected objects in appropriate place
*/
#ifndef SDCONVERTERGRAPHROTATE_H
#define SDCONVERTERGRAPHROTATE_H

#include "SdConverter.h"

#include "SdPoint.h"

class SdConverterGraphRotate : public SdConverter
  {
    SdPoint   mCenter; //!< Center of rotation
    SdPvAngle mAngle;  //!< Angle of rotation
  public:
    SdConverterGraphRotate( SdPoint center, SdPoint from, SdPoint to );

    // SdConverter interface
  public:
    virtual QTransform getMatrix() override;
  };

#endif // SDCONVERTERGRAPHROTATE_H
