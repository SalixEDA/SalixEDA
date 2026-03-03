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
#include "VrmlColor.h"
#include "SdScanerVrml.h"

#include <QRgb>

void VrmlColor::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeedValueFloat( 'f', mRed, QStringLiteral("Need color red") ) ) return;
  if( !scaner->tokenNeedValueFloat( 'f', mGreen, QStringLiteral("Need color green") ) ) return;
  scaner->tokenNeedValueFloat( 'f', mBlue, QStringLiteral("Need color blue") );
  }



quint32 VrmlColor::toInt() const
  {
  return qRgb( mRed * 255, mGreen * 255, mBlue * 255 );
  }



QColor VrmlColor::toColor(float mult) const
  {
  return QColor::fromRgbF( mRed * mult, mGreen * mult, mBlue * mult );
  }
