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

  Variable of 2d region
*/
#ifndef SDSCRIPTVALUEVARIABLE2DREGION_H
#define SDSCRIPTVALUEVARIABLE2DREGION_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariable2dRegion : public SdScriptValueVariable
  {
    Sd2dRegion mValue; //!< 2d region value of variable
  public:
    SdScriptValueVariable2dRegion() : mValue() {}

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char  type() const override { return SD_SCRIPT_TYPE_2D_REGION; }

    //!
    //! \brief to2dRegion Convert object to 2d region
    //! \return           2d region
    //!
    virtual Sd2dRegion to2dRegion() const override { return mValue; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void assign(SdScriptValuePtr src) override { mValue = src->to2dRegion(); }
  };

#endif // SDSCRIPTVALUEVARIABLE2DREGION_H
