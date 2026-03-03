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

  Base class for all unary operations
*/
#ifndef SDSCRIPTVALUEOPUNARY_H
#define SDSCRIPTVALUEOPUNARY_H

#include "SdScriptValue.h"

class SdScriptValueOpUnary : public SdScriptValue
  {
  protected:
    SdScriptValuePtr mOperand; //!< Single operand of operations
  public:
    SdScriptValueOpUnary( SdScriptValuePtr op ) : SdScriptValue(), mOperand(op) {}
    ~SdScriptValueOpUnary() { delete mOperand; }
  };

#endif // SDSCRIPTVALUEOPUNARY_H
