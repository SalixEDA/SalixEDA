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
#include "SdDMasterSheetDecorator.h"

SdDMasterSheetDecorator::SdDMasterSheetDecorator( SdProjectItem *sheet, QWidget *parent ) :
  QDialog(parent),
  mSheet( dynamic_cast<SdPItemSheet*>(sheet) )
  {

  }
