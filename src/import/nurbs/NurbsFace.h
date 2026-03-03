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
#ifndef NURBSFACE_H
#define NURBSFACE_H

#include <QVector3D>

struct NurbsFace
  {
    QVector3D mNormal;
    double    mDisc;
    int       mVertexIndexes[3];
  };

#endif // NURBSFACE_H
