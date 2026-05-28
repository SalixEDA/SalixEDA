/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Layer property
*/
#include "SdPvLayer.h"
#include "SdEnvir.h"
#include "SdJsonIO.h"



//!
//! \brief SdPvLayer Constructor with layer identifier
//! \param id Layer identifier string to find and set the layer
//!
SdPvLayer::SdPvLayer(QString id) :
  SdPv<SdLayerPtr,SdPvLayer>( SdEnvir::instance()->layerGet(id) )
  {

  }

SdPvLayer::SdPvLayer(SdLayer *layer) :
  SdPv<SdLayerPtr,SdPvLayer>(layer)
  {
  if( mValue == nullptr )
    mValue = SdEnvir::instance()->layerGet(LID0_INVISIBLE);
  }




//!
//! \brief isVisible Check if layer is visible
//! \param otherSide If true, check paired layer for flipped components
//! \return True if layer (or its pair) is visible
//!
bool SdPvLayer::isVisible(bool otherSide) const
  {
  SdLayer *lay = layer( otherSide );
  if( lay )
    return lay->isVisible();
  return false;
  }




//!
//! \brief isEdited Check if layer is in edit mode
//! \return True if layer is currently editable
//!
bool SdPvLayer::isEdited() const
  {
  SdLayer *lay = layer( false );
  if( lay )
    return lay->isEdited();
  return false;
  }



//!
//! \brief set Set layer by identifier
//! \param id Layer identifier string to find and set
//!
void SdPvLayer::set(const QString id)
  {
  mValue = SdEnvir::instance()->layerGet( id.isEmpty() ? LID0_INVISIBLE : id );
  }





//!
//! \brief layer Get layer pointer
//! \param otherSide If true, return paired layer for flipped components
//! \return Pointer to SdLayer object
//!
SdLayer *SdPvLayer::layer(bool otherSide) const
  {
  if( otherSide ) return mValue->pair();
  return mValue;
  }







//!
//! \brief setLayerUsage Mark layer as used (sets usage flag)
//!
void SdPvLayer::setLayerUsage() const
  {
  mValue->setUsage();
  mValue->pair()->setUsage();
  }






//!
//! \brief json Read layer property from JSON reader
//! \param name JSON key name
//! \param js   JSON reader object
//!
void SdPvLayer::json(const QString name, const SdJsonReader &js)
  {
  mValue = SdEnvir::instance()->layerGet( js.object().value(name).toString() );
  }




//!
//! \brief replace Replace layer on table base
//! \param pairs   Table of layer pairs. even - source layer id, odd - destignation layer id
//! \return        true if replaced
//!
bool SdPvLayer::replace(const QStringList &pairs)
  {
  for( int i = 0; i < pairs.count(); i += 2 )
    if( layer()->id() == pairs.at(i) ) {
      qDebug() << "Replace " << pairs.at(i) << " on " << pairs.at(i+1);
      set( pairs.at(i+1) );
      return true;
      }
  return false;
  }



