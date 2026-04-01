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

  Append Body with its color to Instance
*/
#ifndef SDSCRIPTVALUEFUNMODELADD_H
#define SDSCRIPTVALUEFUNMODELADD_H
#include "SdScriptValueFunction.h"

class SdScriptValueFunModelAdd : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelAdd() : SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_FACE_LIST ) { }

    //!
    //! \brief toModel Convert object to model which is compound of some bodies
    //! \return        Model which is compound of some bodies
    //!
    virtual Sd3drInstance toModel() const override {
      Sd3drBody body;
      Sd3drMaterial color( mParamList[1]->toColor(), mParamList[1]->toColor(), mParamList[1]->toColor() );
      body.colorListSet( color );
      body.faceAppend( mParamList[2]->toFaceList() );
      Sd3drInstance inst = mParamList[0]->toModel();
      inst.add( body );
      return inst;
      }

  };

#endif // SDSCRIPTVALUEFUNMODELADD_H
