/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Base tool bar for all property bars
*/
#ifndef SDPROPBAR_H
#define SDPROPBAR_H

#include "objects/SdProjectItem.h"

#include <QToolBar>

class SdPropBar : public QToolBar
  {
    Q_OBJECT

  public:
    SdPropBar(const QString title) : QToolBar( title ) {}

  signals:
    //On property changed by user
    void propChanged();

  public slots:
    virtual void setEditObject(SdProjectItem *pitem) { Q_UNUSED(pitem); }
  };


#endif // SDPROPBAR_H
