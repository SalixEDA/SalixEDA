/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Embedded function.

  The function copy model to other position and orientation
*/
#ifndef SDSCRIPTVALUEFUNMODELSOLIDCOPY_H
#define SDSCRIPTVALUEFUNMODELSOLIDCOPY_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelSolidCopy : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelSolidCopy() : SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT )
      {
      }

    //!
    //! \brief toModel Convert object to model which is compound of some bodies
    //! \return        Model which is compound of some bodies
    //!
    virtual Sd3drInstance        toModel() const override
      {
      Sd3drInstance model = mParamList[0]->toModel();
      QMatrix4x4 mat{};
      mat.translate( mParamList[4]->toFloat(), mParamList[5]->toFloat(), mParamList[6]->toFloat() );
      mat.rotate( mParamList[3]->toFloat(), 0, 0, 1.0 );
      mat.rotate( mParamList[2]->toFloat(), 0, 1.0, 0 );
      mat.rotate( mParamList[1]->toFloat(), 1.0, 0, 0 );
      model.addCopy( mat );
      return model;
      }

  };
#endif // SDSCRIPTVALUEFUNMODELSOLIDCOPY_H
