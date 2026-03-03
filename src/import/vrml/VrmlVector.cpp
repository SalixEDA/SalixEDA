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
#include "VrmlVector.h"
#include "SdScanerVrml.h"


void VrmlVector::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeedValueFloat( 'f', mX, QStringLiteral("Need vector X") ) ) return;
  if( !scaner->tokenNeedValueFloat( 'f', mY, QStringLiteral("Need vector X") ) ) return;
  scaner->tokenNeedValueFloat( 'f', mZ, QStringLiteral("Need vector X") );
  }
