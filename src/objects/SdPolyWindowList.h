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

#ifndef SDPOLYWINDOWLIST_H
#define SDPOLYWINDOWLIST_H

#include "SdPolyWindow.h"
#include "SdPointList.h"

#include <QList>

class SdPolyWindowList : public QList<SdPolyWindow>
  {
    SdPointList *mPolygon; //appropriate polygon
  public:
    SdPolyWindowList() : mPolygon(nullptr) {}

    //Reset windows list
    void         reset( SdPointList *poly );

    //Return polygon which window list
    SdPointList *polygon() const { return mPolygon; }

    //Appending service
    void         appendRegion( const QPolygonF &pgn );
    void         appendCircle( SdPoint center, int radius );

    //Test point
    bool         containsPoint( SdPoint p ) const;

  };

#endif // SDPOLYWINDOWLIST_H
