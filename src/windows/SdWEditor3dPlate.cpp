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
#include "SdWEditor3dPlate.h"
#include "objects/SdPItemPlate.h"

SdWEditor3dPlate::SdWEditor3dPlate( SdPItemPlate *item, QWidget *parent ) :
  SdWEditor3d( item, parent ),
  mPlate(item)
  {

  }

void SdWEditor3dPlate::onActivateEditor()
  {
  SdWEditor3d::onActivateEditor();

  mPlate->rebuild3dModel();
  }
