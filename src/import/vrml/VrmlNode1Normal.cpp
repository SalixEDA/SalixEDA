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
#include "VrmlNode1Normal.h"

VrmlNode1Normal::VrmlNode1Normal()
  {

  }


bool VrmlNode1Normal::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  Q_UNUSED(scaner)
  Q_UNUSED(fieldType)
  return false;
  }
