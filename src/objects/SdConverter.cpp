/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Converter form template matrix to convert physical coords into pixels
*/
#include "SdConverter.h"
#include "SdContext.h"


SdConverter::SdConverter() :
  mNext(nullptr),
  mContext(nullptr),
  mPairLayer(false)
  {
  }


SdConverter::~SdConverter()
  {
  if( mContext )
    mContext->removeConverter( this );
  }



//Convert layer flag
bool SdConverter::getFullPairLayer()
  {
  if( mNext )
    return mPairLayer ? !mNext->getFullPairLayer() : mNext->getFullPairLayer();
  return  mPairLayer;
  }




//Calculate and return full transformation matrix
QTransform SdConverter::getFullMatrix()
  {
  QTransform m = getMatrix();
  for( SdConverter *c = mNext; c; c = c->mNext )
    m *= c->getMatrix();
  return m;
  }



double SdConverter::getScale() const
  {
  if( mNext )
    return mNext->getScale();
  return 1.0;
  }




bool SdConverter::getMirror() const
  {
  if( mNext )
    return mNext->getMirror();
  return false;
  }




int SdConverter::getAngle() const
  {
  if( mNext )
    return mNext->getAngle();
  return 0;
  }

