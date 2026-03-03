/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for filled rectangle
*/
#ifndef SDMODECLINEARRECTFILLED_H
#define SDMODECLINEARRECTFILLED_H

#include "SdModeCLinearRect.h"

class SdModeCLinearRectFilled : public SdModeCLinearRect
  {
  public:
    SdModeCLinearRectFilled( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getIndex() const override;
  protected:
    //Insert rectangle into graph object
    virtual void    addRect() override;
  };

#endif // SDMODECLINEARRECTFILLED_H
