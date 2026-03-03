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

  Block of operators executed consiquently as single operator
*/
#include "SdScriptOperatorBlock.h"

SdScriptOperatorBlock::SdScriptOperatorBlock()
  {

  }

SdScriptOperatorBlock::~SdScriptOperatorBlock()
  {
  qDeleteAll( mOperatorList );
  }




//!
//! \brief execute Execute operator
//!
void SdScriptOperatorBlock::execute()
  {
  for( auto ptr : std::as_const(mOperatorList) )
    ptr->execute();
  }
