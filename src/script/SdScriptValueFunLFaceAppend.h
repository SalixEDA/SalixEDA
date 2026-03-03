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

  The function append single face to face list
*/
#ifndef SDSCRIPTVALUEFUNLFACEAPPEND_H
#define SDSCRIPTVALUEFUNLFACEAPPEND_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceAppend : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunLFaceAppend() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE_LIST )
      {

      }


    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override { return mParamList[0]->toFaceList() + mParamList[1]->toFaceList(); }

  };

#endif // SDSCRIPTVALUEFUNLFACEAPPEND_H
