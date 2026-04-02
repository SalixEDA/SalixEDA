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
#include "Sd3drMaterial.h"






void Sd3drMaterial::setAmbientIntensity(float am)
  {
  float fcolor[4];
  colorToFloat( mDiffuse, fcolor );

  for( int i = 0; i < 3; i++ )
    fcolor[i] *= am;

  mAmbient = QColor( fcolor[0] * 255.0, fcolor[1] * 255.0, fcolor[2] * 255.0, fcolor[3] * 255.0 );
  }




void Sd3drMaterial::draw(QOpenGLFunctions_2_0 *f) const
  {
  float fcolor[4];
  colorToFloat( mAmbient, fcolor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, fcolor );

  colorToFloat( mDiffuse, fcolor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, fcolor );

//  Sd3dDraw::colorToFloat( mEmissive, fcolor );
//  f->glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, fcolor );

//  f->glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mShininnes );

//  Sd3dDraw::colorToFloat( mSpecular, fcolor );
//  f->glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, fcolor );
  }



void Sd3drMaterial::setColor(QColor color)
  {
  mDiffuse = color;
  }



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void Sd3drMaterial::json(SvJsonWriter &js) const
  {
  js.jsonColor( QStringLiteral("Ambient"), mAmbient );
  js.jsonColor( QStringLiteral("Diffuse"), mDiffuse );
  js.jsonColor( QStringLiteral("Emissive"), mEmissive );
  js.jsonColor( QStringLiteral("Specular"), mSpecular );
  js.jsonDouble( QStringLiteral("Shininnes"), mShininnes );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void Sd3drMaterial::json(const SvJsonReader &js)
  {
  js.jsonColor( QStringLiteral("Ambient"), mAmbient );
  js.jsonColor( QStringLiteral("Diffuse"), mDiffuse );
  js.jsonColor( QStringLiteral("Emissive"), mEmissive );
  js.jsonColor( QStringLiteral("Specular"), mSpecular );
  js.jsonDouble( QStringLiteral("Shininnes"), mShininnes );
  }



void Sd3drMaterial::colorToFloat(QColor col, float *fcolor)
  {
  fcolor[0] = col.redF();
  fcolor[1] = col.greenF();
  fcolor[2] = col.blueF();
  fcolor[3] = col.alphaF();
  }



//!
//! \brief drawColor Draw both ambient and diffuse colors
//! \param f         OpenGL functions
//! \param color     Color value
//!
void Sd3drMaterial::drawColor(QOpenGLFunctions_2_0 *f, int ambient, int diffuse )
  {
  float fcolor[4];
  fcolor[0] = float((ambient>>16) & 0xff) / 255.0;
  fcolor[1] = float((ambient>>8) & 0xff) / 255.0;
  fcolor[2] = float((ambient>>0) & 0xff) / 255.0;
  fcolor[3] = 1.0;
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, fcolor );

  fcolor[0] = float((diffuse>>16) & 0xff) / 255.0;
  fcolor[1] = float((diffuse>>8) & 0xff) / 255.0;
  fcolor[2] = float((diffuse>>0) & 0xff) / 255.0;
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, fcolor );
  }



