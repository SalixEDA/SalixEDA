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
#ifndef NURBSSURFACEINFO_H
#define NURBSSURFACEINFO_H

#include <QVector3D>
#include <QList>

struct NurbsSurfaceInfo
  {
    QList< QList<QVector3D> > mPoints;
    QList< QList<double> >    mKnots;

    NurbsSurfaceInfo( const QList< QList<QVector3D> > &points, const QList< QList<double> > &knots ) : mPoints(points), mKnots(knots) {}
    NurbsSurfaceInfo() {}

  };

#endif // NURBSSURFACEINFO_H
