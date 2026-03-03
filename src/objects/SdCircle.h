/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Primitive Circle defined by center point and radius
*/

#ifndef SDCIRCLE_H
#define SDCIRCLE_H

#include "SdPoint.h"

class SdRect;

struct SdCircle
  {
    SdPoint mCenter;  //!< Circle center point [Центр окружности]
    int     mRadius;  //!< Circle radius [Радиус окружности]

    SdCircle() : mCenter(), mRadius(0) {}
    SdCircle( SdPoint c, int r ) : mCenter(c), mRadius(r) {}

    bool   isPointInside(SdPoint p) const;    //Истина, если точка внутри окружности
    bool   isRectInside( SdRect r ) const;    //Истина, если прямоугольник внутри окружности
    bool   isCircleInside( SdCircle c) const; //Истина, если окружность внутри окружности

    SdRect overRect() const;           //Описанный квадрат
    SdRect innerRect() const;          //Вписанный квадрат
  };

#endif // SDCIRCLE_H
