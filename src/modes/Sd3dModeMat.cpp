/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Base class for all 3d view modes used matrix mapping
*/
#include "Sd3dModeMat.h"

Sd3dModeMat::Sd3dModeMat(SdPItemPart *part) :
  Sd3dMode(),
  mPart(part)
  {

  }


