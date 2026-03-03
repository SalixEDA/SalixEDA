/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for distance measurement
*/
#ifndef SDMODETEMPRULLER_H
#define SDMODETEMPRULLER_H

#include "SdModeTemp.h"

class SdModeTRuller : public SdModeTemp
  {
    SdPoint mFirst;
    SdPoint mSecond;
    const int sFirst = 0, sSecond = 1;
  public:
    SdModeTRuller( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint enter) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint pos) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  };

#endif // SDMODETEMPRULLER_H
