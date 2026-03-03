/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for draw filled circle's
*/
#ifndef SDMODECLINEARCIRCLEFILLED_H
#define SDMODECLINEARCIRCLEFILLED_H

#include "SdModeCLinearCircle.h"

class SdModeCLinearCircleFilled : public SdModeCLinearCircle
  {
  public:
    SdModeCLinearCircleFilled( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getIndex() const override;

    // SdModeCLinearCircle interface
  protected:
    virtual void    addCircle() override;
  };

#endif // SDMODECLINEARCIRCLEFILLED_H
