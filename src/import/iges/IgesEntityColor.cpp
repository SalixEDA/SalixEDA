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
#include "IgesEntityColor.h"
#include "IgesReader.h"

#include <QDebug>

IgesEntityColor::IgesEntityColor()
  {
  qDebug() << "Entity color";
  }


bool IgesEntityColor::parse(IgesReader *reader)
  {
  if( !IgesEntity::parse( reader ) ) return false;

  double red, green, blue;


  if( !reader->paramReal( red ) ) return false;
  if( !reader->paramReal( green ) ) return false;
  if( !reader->paramReal( blue ) ) return false;

  mColor = QColor( 255 * red, 255 * green, 255 * blue );
  return true;
  }
