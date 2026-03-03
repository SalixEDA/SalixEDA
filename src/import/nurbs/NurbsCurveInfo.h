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
#ifndef NURBSCURVEINFO_H
#define NURBSCURVEINFO_H

#include <QVector3D>
#include <QList>

class NurbsCurveInfo
  {
    QList< QList<QVector3D> > mPoints;
    QList<double>             mKnots;

    NurbsCurveInfo( const QList< QList<QVector3D> > &points, const QList<double> &knots ) : mPoints(points), mKnots(knots) {}
    NurbsCurveInfo() {}
  };

#endif // NURBSCURVEINFO_H
