/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  PCB board builder. In dialog form we create board rectangle
  and fix holes.
*/
#ifndef SDDBOARDBUILDER_H
#define SDDBOARDBUILDER_H

#include "objects/SdPItemPlate.h"

#include <QDialog>
#include <QLineEdit>
#include <QButtonGroup>

class SdDBoardBuilder : public QDialog
  {
    Q_OBJECT

    SdPItemPlate *mPlate;        //!< PCB for witch we create board

    QLineEdit    *mSizeX;        //!< Board size X
    QLineEdit    *mSizeY;        //!< Board size Y
    QLineEdit    *mHoleDiam;     //!< Hole diametr
    QLineEdit    *mHoleDist;     //!< Hole center distance to board edge
    QButtonGroup *mBoardVariant; //!< Board variant selector (hole count)
  public:
    SdDBoardBuilder( SdProjectItem *item, QWidget *parent );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };


#endif // SDDBOARDBUILDER_H
