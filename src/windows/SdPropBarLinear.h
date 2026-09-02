/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Properties bar for lines and other linear objects
*/
#ifndef SDPROPBARLINEAR_H
#define SDPROPBARLINEAR_H

#include "SdPropBarLay.h"
#include "objects/SdPropLine.h"

#include <QComboBox>

#define LINEAR_WIDTH_PREV_COUNT 10

class SdPropBarLinear : public SdPropBarLay
  {
    Q_OBJECT

  protected:
    //Width of line
    double     mPPM;   //Logical coord per physical
    QComboBox *mWidth; //Width of linear objects

    //Vertex type of two lines
    QAction   *mEnterOrtho;
    QAction   *mEnter45degree;
    QAction   *mEnterAnyDegree;

    //Line type
    QAction   *mLineSolid;
    QAction   *mLineDotted;
    QAction   *mLineDashed;
  public:
    SdPropBarLinear( const QString title );

    void setPropLine( const SdPropLine *propLine, double ppm, int enterType );
    void getPropLine( SdPropLine *propLine, int *enterType );

    void setPropLine( const SdPropComposerLine &propLine, double ppm, int enterType );
    void getPropLine(SdPropComposerLine &propLine, int *enterType );

    //!
    //! \brief mBarId Prop Bar id
    //!
    static constexpr int mBarId = PB_LINEAR;

  private:
    void setVertexType( int type );
    void setLineType( int type );
  };


using SdPropBarLinearPtr = SdPropBarPointer<SdPropBarLinear>;

#endif // SDPROPBARLINEAR_H
