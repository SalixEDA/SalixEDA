/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for value of component moving in the schematic sheet
*/
#ifndef SDMODECSHEETVALUEMOVE_H
#define SDMODECSHEETVALUEMOVE_H

#include "SdModeCSheetIdentMove.h"

class SdGraphSymImp;


class SdModeCSheetValueMove : public SdModeCSheetIdentMove
  {
  public:
    SdModeCSheetValueMove(SdWEditorGraph *editor, SdProjectItem *obj);


    // SdMode interface
  public:
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getIndex() const override;

  protected:
    virtual void    setProp( const SdPropText &prp, SdPoint pos, SdUndo *undo ) override;
    virtual void    getProp() override;
  };

#endif // SDMODECSHEETVALUEMOVE_H
