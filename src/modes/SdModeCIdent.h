/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for change position and properties of ident
*/
#ifndef SDMODECIDENT_H
#define SDMODECIDENT_H

#include "SdModeCTextual.h"

class SdGraphIdent;

class SdModeCIdent : public SdModeCTextual
  {
    SdGraphIdent *mIdent; //Edited ident
    int           mIndex; //Mode index
  public:
    SdModeCIdent( SdWEditorGraph *editor, SdProjectItem *obj, SdPropText *prp, int index );

    // SdMode interface
  public:
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getIndex() const override;
    virtual void    activate() override;
    virtual int     getCursor() const override;

    // SdModeCTextual interface
  protected:
    virtual void    cancelEdit() override;
  };

#endif // SDMODECIDENT_H
