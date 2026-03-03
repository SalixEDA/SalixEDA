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

  The function duplicate face with conversion matrix
*/
#ifndef SDSCRIPTVALUEFUNFACEBEVELXY_H
#define SDSCRIPTVALUEFUNFACEBEVELXY_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceBevelXY : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    SdScriptValueFunFaceBevelXY( Sd3drModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FLOAT ), mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return mModel->faceBevelXY( mParamList[0]->toFace(), mParamList[1]->toFloat() ); }
  };

#endif // SDSCRIPTVALUEFUNFACEBEVELXY_H
