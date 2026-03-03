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

  Boolean inversion
*/
#ifndef SDSCRIPTVALUEOPUNARYBOOLNOT_H
#define SDSCRIPTVALUEOPUNARYBOOLNOT_H

#include "SdScriptValueOpUnary.h"

class SdScriptValueOpUnaryBoolNot : public SdScriptValueOpUnary
  {
  public:
    SdScriptValueOpUnaryBoolNot( SdScriptValuePtr op ) : SdScriptValueOpUnary( op ) {}

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char type() const override { return SD_SCRIPT_TYPE_BOOL; }

    //!
    //! \brief toBool Convert object to bool value
    //! \return       bool value
    //!
    virtual bool toBool() const override { return !mOperand->toBool(); }
  };

#endif // SDSCRIPTVALUEOPUNARYBOOLNOT_H
