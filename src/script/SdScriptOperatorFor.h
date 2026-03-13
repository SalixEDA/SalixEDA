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
#ifndef SDSCRIPTOPERATORFOR_H
#define SDSCRIPTOPERATORFOR_H

#include "SdScriptOperator.h"
#include "SdScriptValueVariableFloat.h"

class SdScriptOperatorFor : public SdScriptOperator
  {
    SdScriptValueVariableFloat *mLoopIndex;   //!< Variable which is loop index
    SdScriptValue              *mLoopCount; //!< Expression loop count
    SdScriptOperator           *mBlock;     //!< Operator to execute
  public:
    SdScriptOperatorFor( SdScriptValueVariableFloat *var, SdScriptValue *loopCount, SdScriptOperator *block ) :
      mLoopIndex(var),
      mLoopCount(loopCount),
      mBlock(block) {}
    ~SdScriptOperatorFor()
      {
      delete mLoopCount;
      delete mBlock;
      }

    // SdM3dOperator interface
  public:
    //!
    //! \brief execute Execute operator
    //!
    virtual void execute() override;
  };

#endif // SDSCRIPTOPERATORFOR_H
