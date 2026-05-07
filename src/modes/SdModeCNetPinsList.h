/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for insert and edit net pins list
*/
#ifndef SDMODECNETPINSLIST_H
#define SDMODECNETPINSLIST_H

#include "SdMode.h"

class SdGraphNetPinsList;

class SdModeCNetPinsList : public SdMode
  {
    SdPropText         *mPropText;    //!< Text properties
    SdGraphNetPinsList *mNetPinsList; //!< Current netPinsList object
  public:
    SdModeCNetPinsList( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual int getPropBarId() const override;
    virtual void enterPoint(SdPoint) override;
    virtual void cancelPoint(SdPoint) override;
    virtual void movePoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int getCursor() const override;
    virtual int getIndex() const override;
  };

#endif // SDMODECNETPINSLIST_H
