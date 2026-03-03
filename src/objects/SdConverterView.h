/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Convertor for view point
*/
#ifndef SDCONVERTERVIEW_H
#define SDCONVERTERVIEW_H

#include "SdConverter.h"
#include "SdPoint.h"

class SdConverterView : public SdConverter
  {
    SdPoint mCenter; //!< View center in pixels
    double  mScale;  //!< View scale
    SdPoint mOrigin; //!< View offset
    bool    mMirror; //!< When true view is mirrored on x axiz
  public:
    SdConverterView( QSize viewSize, SdPoint origin, double scale, bool mirror );

    //Return local conversion transformation matrix
    virtual QTransform getMatrix() override;

    //Return scale factor
    virtual double     getScale() const override { return mScale; }
  };

#endif // SDCONVERTERVIEW_H
