/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Plate net. Contains list of all net segments.
  On list base we build ratnet.
*/
#ifndef SDPLATENET_H
#define SDPLATENET_H

#include "SdPoint.h"
#include "SdPvStratum.h"
#include "SdPlateNetSegment.h"
#include "SdRatNet.h"

#include <QList>


class SdPlateNet
  {
    QList<SdPlateNetSegment> mSegmentList; //Net segments for all subnets
    int                      mSubNetIndex; //Sub net index for next subnet
  public:
    SdPlateNet();

    //Append one net segment. Check and manage subnets
    void    addNetSegment(SdPvStratum s, SdPoint p1, SdPoint p2);

    //Build ratnet as pair for each-to-other subnets
    void    buildRatNet( SdRatNet *ratNet ) const;

    //Neares point
    SdPoint nearestPoint( SdPoint p );

  private:
    //For each points if point subnet equals oldSubNet then replace on newSubNet
    void replaceSubNet( int oldSubNet, int newSubNet );
  };


#endif // SDPLATENET_H
