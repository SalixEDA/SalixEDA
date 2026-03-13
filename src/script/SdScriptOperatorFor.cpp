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

  Operator for( variableLoopIndex : expressionLoopCount ) block
*/
#include "script/SdScriptOperatorFor.h"


void SdScriptOperatorFor::execute()
  {
  mLoopIndex->reset();
  //Exclude infinite loop. We limit it with 10000 iterations
  for( int i = 0; i < 10000 && mLoopIndex->toFloat() < mLoopCount->toFloat(); i++ ) {
    mBlock->execute();
    mLoopIndex->inc();
    //qDebug() << "Iteration " << i;
    }
  }
