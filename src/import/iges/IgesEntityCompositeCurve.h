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
#ifndef IGESENTITYCOMPOSITECURVE_H
#define IGESENTITYCOMPOSITECURVE_H

#include "IgesEntity.h"

class IgesEntityCompositeCurve : public IgesEntity
  {
    IgesEntityPtrList mList;
  public:
    IgesEntityCompositeCurve();

    // IgesEntity interface
  public:
    virtual bool parse(IgesReader *reader) override;
  };

#endif // IGESENTITYCOMPOSITECURVE_H
