/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  With this mode user fix print window
*/
#ifndef SDMODEPRINTWINDOW_H
#define SDMODEPRINTWINDOW_H

#include "SdModeTemp.h"
#include "objects/SdRect.h"

class SdModePrintWindow : public SdModeTemp
  {
    SdPoint mFirst;
    SdPoint mPrevMove;
    enum { sFirstCorner, sSecondCorner };
  public:
    SdModePrintWindow( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint enter) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  };

#endif // SDMODEPRINTWINDOW_H
