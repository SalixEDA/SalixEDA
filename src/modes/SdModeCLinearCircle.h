/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for draw circle's
*/
#ifndef SDMODECLINEARCIRCLE_H
#define SDMODECLINEARCIRCLE_H

#include "SdModeCLinear.h"

class SdModeCLinearCircle : public SdModeCLinear
  {
  protected:
    SdPoint    mCenter;          //Circle center [Цент окружности]
    int        mRadius;          //Circle radius [Радиус окружности]

    SdPoint    mSmartPoint;      //Smart mode point [Точка разумного ввода]
    SdSnapMask mSmartType;       //Smart point type [Тип разумной точки]

    static int mPreviousRadius;  //Previous circle radius [Радиус предыдущей окружности]
    enum { sCenter, sRadius, sLast };
  public:
    SdModeCLinearCircle( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint p) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual SdPoint enterPrev() override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  protected:
    virtual void    addCircle();
  };

#endif // SDMODECLINEARCIRCLE_H
