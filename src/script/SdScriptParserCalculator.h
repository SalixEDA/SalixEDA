/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
*/
#ifndef SDSCRIPTPARSERCALCULATOR_H
#define SDSCRIPTPARSERCALCULATOR_H

#include "SdScriptParser.h"
#include "SdScriptRefMap.h"


class SdScriptParserCalculator : public SdScriptParser
  {
    SdScriptRefMap *mRefMap; //!< Mapping variables for visual editing
  public:
    SdScriptParserCalculator( SdScriptRefMap *refMap,  QTableWidget *tableWidget );

    // SdScriptParser interface
  protected:
    virtual SdScriptValueVariablePtr buildRefVariable( const QString &name, SdScriptValue *expr ) override;
  };

#endif // SDSCRIPTPARSERCALCULATOR_H
