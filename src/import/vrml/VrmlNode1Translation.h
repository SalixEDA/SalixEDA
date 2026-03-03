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
#ifndef VRMLNODE1TRANSLATION_H
#define VRMLNODE1TRANSLATION_H

#include "VrmlNode1.h"

class VrmlNode1Translation : public VrmlNode1
  {
  public:
    VrmlNode1Translation();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1TRANSLATION_H
