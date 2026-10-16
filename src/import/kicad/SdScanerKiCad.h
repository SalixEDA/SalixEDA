/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
*/
#ifndef SDSCANERKICAD_H
#define SDSCANERKICAD_H

#include "../SdScanerSe.h"

#include <QPointF>
#include <QMap>
#include <functional>

class SdProject;
class SdPItemPart;
class SdProjectItem;
class SdPoint;
class SdPvInt;
class SdPvLayer;
struct SdPropLine;

class SdScanerKiCad;

class SdScanerKiCad : public SdScanerSe
  {
    double mPpm; //!< Conversion factor
  public:
    SdScanerKiCad();

    double ppm() const { return mPpm; }

    void   ppmSet( double ppm ) { mPpm = ppm; }

    int    convert( double v ) const { return v * mPpm; }

    void   parseTop( SdProject *project );

    int tokenNeedCoord() { return convert(tokenNeedValueDouble()); }

    int tokenNeedAngle() { return tokenNeedValueDouble() * 1000.0; }

    void tokenNeedClose() { tokenNeed(')'); }
  };




#endif // SDSCANERKICAD_H
