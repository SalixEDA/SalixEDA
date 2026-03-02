/*
Project "Electronic schematic and pcb CAD"

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


