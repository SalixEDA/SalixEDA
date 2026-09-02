/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Properties bar for tracing road

  Helper properties bar for vias
*/
#ifndef SDPROPBARROAD_H
#define SDPROPBARROAD_H

#include "SdPropBarStratum.h"
#include "objects/SdPropRoad.h"
#include "objects/SdPropVia.h"

#define ROAD_WIDTH_PREV_COUNT 10

class SdPropBarRoad : public SdPropBarStratum
  {
    Q_OBJECT

  protected:
    //Width of line
    double     mPPM;               //Logical coord per physical
    QComboBox *mWidth;             //Width of linear objects

    QLineEdit *mWireName;          //Name of net for road

    //Cursor align to greed or not
    QAction   *mAlignToGrid;

    //Enable-disable automatic road loop detection and removing
    QAction   *mLoopDetection;

    //Vertex type of two lines
    QAction   *mEnterOrtho;
    QAction   *mEnter45degree;
    QAction   *mEnterAnyDegree;

    //Vias
    QAction     *mViaThrough;      //Via through or blind
    QComboBox   *mViaPadType;      //Via pad type


  public:
    SdPropBarRoad(const QString title, bool asRoad = true );

    void setPropRoad( const SdPropRoad &propRoad, const SdPropVia &propVia, double ppm, int enterType );
    void getPropRoad( SdPropRoad &propRoad, SdPropVia &propVia, int *enterType );

    void setPropRoad( const SdPropComposerRoad &propRoad, const SdPropComposerVia &propVia, double ppm, int enterType );
    void getPropRoad( SdPropComposerRoad &propRoad, SdPropComposerVia &propVia, int *enterType );

    void setPropVia( SdPropVia *propVia );
    void getPropVia( SdPropVia *propVia );

    void setViaRule( int viaRule );
    int  getViaRule();

    //!
    //! \brief mBarId Prop Bar id
    //!
    static constexpr int mBarId = PB_ROAD;
  private:
    void setVertexType( int type );
  };

using SdPropBarRoadPtr = SdPropBarPointer<SdPropBarRoad>;



//!
//! \brief The SdPropBarVia class Helper class to distinct via from road
//!
class SdPropBarVia : public SdPropBarRoad
  {
    Q_OBJECT

  public:
    SdPropBarVia( const QString title ) : SdPropBarRoad( title, false ) {}

    //!
    //! \brief mBarId Prop Bar id
    //!
    static constexpr int mBarId = PB_VIA;
  };

using SdPropBarViaPtr = SdPropBarPointer<SdPropBarVia>;

#endif // SDPROPBARROAD_H
