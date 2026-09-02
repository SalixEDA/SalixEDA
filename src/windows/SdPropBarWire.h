/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Properties bar for wires
*/
#ifndef SDPROPBARWIRE_H
#define SDPROPBARWIRE_H

#include "SdPropBarLay.h"
#include "objects/SdPropLine.h"

#include <QComboBox>

#define WIRE_WIDTH_PREV_COUNT 10
#define WIRE_NAME_PREV_COUNT  10


class SdPropBarWire : public SdPropBarLay
  {
    Q_OBJECT

  protected:
    //Width of line
    double     mPPM;   //Logical coord per physical
    QComboBox *mWidth; //Width of linear objects

    QComboBox *mWireName; //Name of nets for wires

    //Vertex type of two lines
    QAction   *mEnterOrtho;
    QAction   *mEnter45degree;
    QAction   *mEnterAnyDegree;

    //Line type
    QAction   *mLineSolid;
    QAction   *mLineDotted;
    QAction   *mLineDashed;
  public:
    SdPropBarWire( const QString title );

    void setPropWire( const SdPropLine &propLine, double ppm, int enterType, const SdPvMulty<SdPvString> wireName );
    void getPropWire( SdPropLine &propLine, int *enterType, SdPvMulty<SdPvString> &wireName );

    void setPropWire(SdPropComposerLine &propLine, double ppm, int enterType, const SdPvMulty<SdPvString> wireName );
    void getPropWire( SdPropComposerLine &propLine, int *enterType, SdPvMulty<SdPvString> &wireName );

    //!
    //! \brief mBarId Prop Bar id
    //!
    static constexpr int mBarId = PB_WIRE;
  private:
    void setVertexType( int type );
    void setLineType( int type );
  };

using SdPropBarWirePtr = SdPropBarPointer<SdPropBarWire>;

#endif // SDPROPBARWIRE_H
