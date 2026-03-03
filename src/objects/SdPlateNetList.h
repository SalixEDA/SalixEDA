/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Plate net list. Contains list of all named nets.
  On list base we build ratnet.
*/
#ifndef SDPLATENETLIST_H
#define SDPLATENETLIST_H

#include "SdRatNet.h"
#include "SdPlateNet.h"
#include "SdPlateNetContainer.h"

#include <QMap>
#include <QString>

//List of plate nets
class SdPlateNetList : public SdPlateNetContainer {
    QMap<QString,SdPlateNet*> mNetList; //Net list
  public:
    SdPlateNetList();
    ~SdPlateNetList();

    //Add net segment to appropriate net
    void    addNetSegment( SdGraphTraced *traced, const QString netName, SdPvStratum s, SdPoint p1, SdPoint p2);

    //For each net build ratnet
    void    buildRatNet( SdRatNet *ratNet );

    //Clear net list
    void    clear();

    //Find nearest point for netName in net list
    SdPoint nearestPoint( const QString netName, SdPoint p );
  };


#endif // SDPLATENETLIST_H
