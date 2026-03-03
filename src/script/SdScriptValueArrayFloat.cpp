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

  Array of floats
*/
#include "SdScriptValueArrayFloat.h"




//!
//! \brief toIndex Convert object to float list
//! \return        Float list
//!
QList<float> SdScriptValueArrayFloat::toFloatList() const
  {
  QList<float> list;
  list.reserve( mArray.count() );
  for( auto ptr : mArray )
    list.append( ptr->toFloatList() );
  return list;
  }
