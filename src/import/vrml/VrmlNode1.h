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
#ifndef VRMLNODE1_H
#define VRMLNODE1_H

#include "VrmlNode.h"

class VrmlNode1 : public VrmlNode
  {
  public:
    VrmlNode1() : VrmlNode() {}

    static VrmlNode  *parse1Node( SdScanerVrml *scaner, const QString &nodeType );
    static VrmlNode  *build1Node( const QString &nodeType );

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return nullptr; }
  };

#endif // VRMLNODE1_H
