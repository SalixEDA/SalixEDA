/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Default tool bar for selection
*/
#ifndef SDPROPBARDEFAULT_H
#define SDPROPBARDEFAULT_H

#include "SdPropBarLay.h"

class SdPropBarDefault : public SdPropBarLay
  {
    Q_OBJECT

    QAction *mMaskComp;    //Do not tought components
    QAction *mMaskNet;     //Do not tought nets
    QAction *mMaskPicture; //Do not tought picture elements
  public:
    SdPropBarDefault(const QString title);
  };

#endif // SDPROPBARDEFAULT_H
