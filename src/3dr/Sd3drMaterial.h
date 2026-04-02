/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  3D face material. Material of which face is made
*/
#ifndef SD3DRMATERIAL_H
#define SD3DRMATERIAL_H


#include "SvLib/SvJsonIO.h"

#include <QJsonObject>
#include <QOpenGLFunctions_2_0>
#include <QList>


struct Sd3drMaterial
  {
    QColor        mAmbient;
    QColor        mDiffuse;
    QColor        mSpecular;
    QColor        mEmissive;
    double        mShininnes;

    Sd3drMaterial() : mShininnes(0) {}

    Sd3drMaterial( QColor amb, QColor dif, QColor spec ) : mAmbient(amb), mDiffuse(dif), mSpecular(spec), mEmissive(0,0,0), mShininnes(0.2) {}

    void setAmbientIntensity( float am );

    void draw( QOpenGLFunctions_2_0 *f ) const;

    void setColor( QColor color );


    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void json( const SvJsonReader &js);

    //!
    //! \brief colorToFloat Convert QColor to array of four float color components
    //! \param col          Source color
    //! \param fcolor       Destignation array of float, must be at least 4 item
    //!
    static void colorToFloat( QColor col, float *fcolor );

    //!
    //! \brief drawColor Draw both ambient and diffuse colors
    //! \param f         OpenGL functions
    //! \param ambient   Color value
    //! \param diffuse   Color value
    //!
    static void drawColor(QOpenGLFunctions_2_0 *f, int ambient , int diffuse);
  };

using Sd3ColorList = QList<Sd3drMaterial>;

#endif // SD3DRMATERIAL_H
