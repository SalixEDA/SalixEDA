/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  3d model programming language

  Build new Instance with initial Body with color
*/
#ifndef SDSCRIPTVALUEFUNMODELNEW_H
#define SDSCRIPTVALUEFUNMODELNEW_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelNew : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelNew() : SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_MATRIX ) { }

    //!
    //! \brief toModel Convert object to model which is compound of some bodies
    //! \return        Model which is compound of some bodies
    //!
    virtual Sd3drInstance toModel() const override {
      Sd3drBody body;
      Sd3drMaterial color( mParamList[0]->toColor(), mParamList[0]->toColor(), mParamList[0]->toColor() );
      body.colorListSet( color );
      body.faceAppend( mParamList[1]->toFaceList() );
      Sd3drInstance inst;
      inst.add( body );
      inst.addCopy( mParamList[2]->toMatrix() );
      return inst;
      }

  };

#endif // SDSCRIPTVALUEFUNMODELNEW_H
