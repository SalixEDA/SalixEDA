/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Base class for part masters.

  Contains operations for append graphics and pins to part when creation.
*/
#include "SdDMasterPart.h"




SdDMasterPart::SdDMasterPart(SdProjectItem *item, QWidget *parent) :
  QDialog( parent ),
  mMasterPart(item)
  {
  //Clear item
  item->clear();
  }


