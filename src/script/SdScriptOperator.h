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

  Base for all operators of script language
*/
#ifndef SDSCRIPTOPERATOR_H
#define SDSCRIPTOPERATOR_H


class SdScriptOperator
  {
  public:
    SdScriptOperator() {}
    virtual ~SdScriptOperator() {}

    //!
    //! \brief execute Execute operator
    //!
    virtual void execute() = 0;
  };

using SdScriptOperatorPtr = SdScriptOperator*;

#endif // SDSCRIPTOPERATOR_H
