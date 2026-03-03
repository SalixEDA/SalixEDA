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

  Binary operation for comparing to less of two's float
*/
#ifndef SDSCRIPTVALUEOPBINARYFLOATLESS_H
#define SDSCRIPTVALUEOPBINARYFLOATLESS_H

#include "SdScriptValueOpBinaryFloat.h"

class SdScriptValueOpBinaryFloatLess : public SdScriptValueOpBinaryFloat
  {
  public:
    SdScriptValueOpBinaryFloatLess( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinaryFloat( first, second ) { }

    // SdM3dValue interface
  public:
    virtual char type() const override { return SD_SCRIPT_TYPE_BOOL; }
    virtual bool toBool() const override { return mFirstOperand->toFloat() < mSecondOperand->toFloat(); }
  };

#endif // SDSCRIPTVALUEOPBINARYFLOATLESS_H
