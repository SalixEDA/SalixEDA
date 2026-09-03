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
#include "SdConverterGraphRotate.h"


SdConverterGraphRotate::SdConverterGraphRotate(SdPoint center, SdPoint from, SdPoint to)
  : mCenter(center)
  , mAngle(0)
  {
  if( center != from && center != to && from != to )
    mAngle = center.getAngleBetween( from, to );
  }

QTransform SdConverterGraphRotate::getMatrix()
  {
  return mCenter.transformRotation( mAngle );
  }
