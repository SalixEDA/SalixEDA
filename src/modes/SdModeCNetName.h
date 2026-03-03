/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for placement net names
*/
#ifndef SDMODECWIRENAME_H
#define SDMODECWIRENAME_H

#include "SdModeCommon.h"

class SdPItemSheet;


class SdModeCNetName : public SdModeCommon
  {
    QString              mNetName;     //Net name

    static SdPoint       mSmartOrigin; //Offset of net name position
    enum { sSelectNet, sPlaceName };
  public:
    SdModeCNetName( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint p) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  private:
    SdPItemSheet        *getSheet();

  };

#endif // SDMODECWIRENAME_H
