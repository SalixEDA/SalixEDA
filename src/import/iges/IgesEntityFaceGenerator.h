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
#ifndef IGESENTITYFACEGENERATOR_H
#define IGESENTITYFACEGENERATOR_H

#include "IgesEntity.h"

class IgesEntityFaceGenerator : public IgesEntity
  {
  public:
    IgesEntityFaceGenerator();

    virtual void generateFaces( Sd3drModel *model ) = 0;
  };

#endif // IGESENTITYFACEGENERATOR_H
