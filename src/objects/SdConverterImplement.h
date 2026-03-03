/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Converter for symbol and part implementation
*/
#ifndef SDCONVERTERIMPLEMENT_H
#define SDCONVERTERIMPLEMENT_H

#include "SdConverter.h"
#include "SdPoint.h"

class SdConverterImplement : public SdConverter
  {
    SdPoint mPosition; //Position of implement
    SdPoint mOrigin;   //Origin of implement
    double  mAngle;    //Angle of rotation
    bool    mMirror;   //True if implement mirrored
  public:
    SdConverterImplement( SdPoint pos, SdPoint org, SdPvAngle angle, bool mirror );

    // SdConverter interface
  public:
    virtual QTransform getMatrix() override;
    virtual bool       getMirror() const override;
    virtual int        getAngle() const override;
  };

#endif // SDCONVERTERIMPLEMENT_H
