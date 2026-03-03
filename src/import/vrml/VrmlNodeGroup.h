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
#ifndef VRMLNODEGROUP_H
#define VRMLNODEGROUP_H

#include "VrmlVector.h"
#include "VrmlNodeCompound.h"

class VrmlNodeGroup : public VrmlNodeCompound
  {
    VrmlVector mBoxCenter;
    VrmlVector mBoxSize;
  public:
    VrmlNodeGroup();
    VrmlNodeGroup( const VrmlNodeGroup *group );

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeGroup( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODEGROUP_H
