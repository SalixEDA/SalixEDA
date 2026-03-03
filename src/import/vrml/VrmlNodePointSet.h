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
#ifndef VRMLNODEPOINTSET_H
#define VRMLNODEPOINTSET_H

#include "VrmlNode.h"

class VrmlNodePointSet : public VrmlNode
  {
    VrmlNode           *mColor;
    VrmlNode           *mCoordinate;
  public:
    VrmlNodePointSet();
    VrmlNodePointSet( const VrmlNodePointSet *ps );
    virtual ~VrmlNodePointSet();


    // VrmlNode interface
  public:
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
    virtual VrmlNode *copy() const override;
  };

#endif // VRMLNODEPOINTSET_H
