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

  Build new Instance with initial Body with color, rotation and offset
*/
#ifndef SDSCRIPTVALUEFUNMODELSOLID_H
#define SDSCRIPTVALUEFUNMODELSOLID_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelSolid : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelSolid() : SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

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
      QMatrix4x4 mat{};
      mat.translate( mParamList[5]->toFloat(), mParamList[6]->toFloat(), mParamList[7]->toFloat() );
      mat.rotate( mParamList[4]->toFloat(), 0, 0, 1.0 );
      mat.rotate( mParamList[3]->toFloat(), 0, 1.0, 0 );
      mat.rotate( mParamList[2]->toFloat(), 1.0, 0, 0 );
      inst.addCopy( mat );
      return inst;
      }

  };
#endif // SDSCRIPTVALUEFUNMODELSOLID_H
