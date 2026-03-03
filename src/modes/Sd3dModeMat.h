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
#ifndef SD3DMODEMAT_H
#define SD3DMODEMAT_H

#include "objects/SdPItemPart.h"
#include "Sd3dMode.h"

#include <QMatrix4x4>

class Sd3dModeMat : public Sd3dMode
  {
  protected:
    SdPItemPart *mPart;   //!< Part on which applied this mode
  public:
    Sd3dModeMat( SdPItemPart *part );

  };

#endif // SD3DMODEMAT_H
