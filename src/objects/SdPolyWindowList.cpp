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

#include "SdPolyWindowList.h"
#include "SdUtil.h"

#include <QJsonArray>
#include <QPolygonF>


//Reset windows list
void SdPolyWindowList::reset(SdPointList *poly)
  {
  clear();
  mPolygon = poly;
  }



void SdPolyWindowList::appendRegion(const QPolygonF &pgn)
  {
  //We append only 4 vertex regions
  if( pgn.count() == 4 ) {
    if( mPolygon && pgn.intersects( *mPolygon ) ) {
      SdPolyWindow win( pgn );
      append( win );
      }
    }
  }




void SdPolyWindowList::appendCircle(SdPoint center, int radius)
  {
  QPolygonF pgn = SdUtil::octagon( center.x(), center.y(), radius );
  if( mPolygon && pgn.intersects( *mPolygon ) ) {
    SdPolyWindow win( center, radius );
    append( win );
    }
  }




//Test point
bool SdPolyWindowList::containsPoint(SdPoint p) const
  {
  for( const SdPolyWindow &win : *this )
    if( win.containsPoint(p) ) return true;
  return false;
  }




