/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Sheet decorator master.

  Appends sheet decoration to new created sheet
*/
#ifndef SDDMASTERSHEETDECORATOR_H
#define SDDMASTERSHEETDECORATOR_H

#include "objects/SdPItemSheet.h"

#include <QDialog>

class SdDMasterSheetDecorator : public QDialog
  {
    Q_OBJECT

    SdPItemSheet *mSheet;
  public:
    SdDMasterSheetDecorator( SdProjectItem *sheet, QWidget *parent );
  };

#endif // SDDMASTERSHEETDECORATOR_H
