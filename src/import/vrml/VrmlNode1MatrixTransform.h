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
#ifndef VRMLNODE1MATRIXTRANSFORM_H
#define VRMLNODE1MATRIXTRANSFORM_H

#include "VrmlNode1.h"

#include <QMatrix4x4>

class VrmlNode1MatrixTransform : public VrmlNode1
  {
    QMatrix4x4 mMatrix;
  public:
    VrmlNode1MatrixTransform();

    QMatrix4x4 matrix() const { return mMatrix; }

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1MATRIXTRANSFORM_H
