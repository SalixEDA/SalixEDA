/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
*/

#ifndef SDWVIEW_H
#define SDWVIEW_H

#include "objects/SdPoint.h"
#include "objects/SdRect.h"
#include "objects/SdScaler.h"
#include "modes/SdMode.h"
#include <QWidget>

class SdWView : public QWidget
  {
    Q_OBJECT

  public:
    explicit SdWView(QWidget *parent = nullptr);

  signals:

  public slots:
  };

#endif // SDWVIEW_H
