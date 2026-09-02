/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Properties bar for tracing polygon
*/
#ifndef SDPROPBARPOLYGON_H
#define SDPROPBARPOLYGON_H

#include "SdPropBarStratum.h"
#include "objects/SdPropPolygon.h"

#define POLYGON_GAP_PREV_COUNT 10


class SdPropBarPolygon : public SdPropBarStratum
  {
    Q_OBJECT

  protected:
    //Width of line
    double     mPPM;      //Logical coord per physical
    QComboBox *mGap;      //Gap for polygon windows

    QComboBox *mWireName; //Name of net for polygon

    //Vertex type of two lines
    QAction   *mEnterOrtho;
    QAction   *mEnter45degree;
    QAction   *mEnterAnyDegree;

  public:
    SdPropBarPolygon( const QString title );

    void setPropPolygon( const SdPropPolygon &propPolygon, double ppm, int enterType , const QStringList list);
    void getPropPolygon( SdPropPolygon &propPolygon, int *enterType );

    void setPropPolygon( const SdPropComposerPolygon &propPolygon, double ppm, int enterType , const QStringList list);
    void getPropPolygon( SdPropComposerPolygon &propPolygon, int *enterType );

    //!
    //! \brief mBarId Prop Bar id
    //!
    static constexpr int mBarId = PB_POLYGON;
  private:
    void setVertexType( int type );
  };

using SdPropBarPolygonPtr = SdPropBarPointer<SdPropBarPolygon>;

#endif // SDPROPBARPOLYGON_H
