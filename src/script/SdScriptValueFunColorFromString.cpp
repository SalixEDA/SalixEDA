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

  Build color structure from string description
*/
#include "SdScriptValueFunColorFromString.h"


SdScriptValueFunColorFromString::SdScriptValueFunColorFromString() :
  SdScriptValueFunction( SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_STRING )
  {

  }


//!
//! \brief toColor Convert object to color
//! \return        Color
//!
QColor SdScriptValueFunColorFromString::toColor() const
  {
  return QColor( mParamList[0]->toString() );
  }
