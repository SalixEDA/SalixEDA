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

  Binary operation for mult two's float
*/
#ifndef SDSCRIPTVALUEOPBINARYFLOATMULT_H
#define SDSCRIPTVALUEOPBINARYFLOATMULT_H

#include "SdScriptValueOpBinaryFloat.h"

class SdScriptValueOpBinaryFloatMult : public SdScriptValueOpBinaryFloat
  {
  public:
    SdScriptValueOpBinaryFloatMult( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinaryFloat( first, second ) {}

    // SdM3dValue interface
  public:
    virtual float toFloat() const override { return mFirstOperand->toFloat() * mSecondOperand->toFloat(); }
  };

#endif // SDSCRIPTVALUEOPBINARYFLOATMULT_H
