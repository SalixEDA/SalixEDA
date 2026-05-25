/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Base tool bar for property bars
*/
#include "SdPropBarLay.h"
#include "SdWCommand.h"
#include "objects/SdEnvir.h"
#include "objects/SdPulsar.h"

#include <QStyleFactory>
#include <QTimer>

SdPropBarLay::SdPropBarLay( const QString title ) :
  SdPropBar( title ),
  mEditObjectClass(dctCommon),
  mStratum(stmThrough)
  {
  insertAction( nullptr, SdWCommand::cmViewLayers );
  mLayer = new QComboBox();
  mLayer->setStyle( QStyleFactory::create("windows") );
  //mLayer->setEditable( true );
  mLayer->setMaxVisibleItems( 10 );
  addWidget( mLayer );
  mLayer->setMinimumWidth( 150 );
  mLayer->setToolTip( tr("Current layer") );

  //current layer selection changed. Send signal "prop changed"
  connect( mLayer, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, [this](int index) {
    Q_UNUSED(index)
    emit propChanged();
    });
  //When layer dialog completed
  connect( SdPulsar::sdPulsar, &SdPulsar::viewedLayers, this, &SdPropBarLay::updateViewedLayers );

  //fill new layers list
  QTimer::singleShot( 20, this, [this] () { updateViewedLayers(nullptr); } );
  }




void SdPropBarLay::setSelectedLayer(SdLayer *layer)
  {
  if( layer != nullptr ) {
    QString id = layer->id();
    int index = mLayer->findData( QVariant(id) );
    if( index >= 0 )
      mLayer->setCurrentIndex( index );
    else {
      //Switch on requred layer
      layer->stateSet( layerStateEdit );
      SdPulsar::sdPulsar->emitViewedLayers( layer );
      // updateViewedLayers( layer );
      // index = mLayer->findData( QVariant(id) );
      // if( index >= 0 )
      //   mLayer->setCurrentIndex( index );
      // else
      //   qDebug() << "setSelectedLayer failure";
      }
    }
  else {
    int index = mLayer->findText( QString() );
    if( index >= 0 )
      mLayer->setCurrentIndex( index );
    else
      mLayer->setCurrentIndex( -1 );
    }
  }




SdLayer *SdPropBarLay::getSelectedLayer()
  {
  int index = mLayer->currentIndex();
  if( index < 0 )
    return nullptr;
  QString id = mLayer->currentData().toString();
  if( id.isEmpty() )
    return nullptr;
  return SdEnvir::instance()->layerGet( id );
  }




//Visibility state of layers are changed. We need update list and preserve current layer selection
void SdPropBarLay::updateViewedLayers(SdLayer *currentLayer)
  {
  SdLayer *prevLayer = getSelectedLayer();
  //get current selection
  if( currentLayer == nullptr )
    currentLayer = prevLayer;
  currentLayer = refillLayers( currentLayer );
  setSelectedLayer( currentLayer );

  if( prevLayer != currentLayer )
    emit propChanged();
  }





void SdPropBarLay::setEditObject(SdProjectItem *pitem)
  {
  if( pitem == nullptr )
    setEditObject( dctCommon, stmThrough );
  else
    setEditObject( pitem->getClass(), pitem->getStratum() );
  }




void SdPropBarLay::setEditObject(SdClass theClass, SdPvStratum stratum)
  {
  if( mEditObjectClass != theClass || mStratum != stratum )  {
    mEditObjectClass = theClass;
    mStratum         = stratum;
    refillLayers(nullptr);
    }
  }



//!
//! \brief refillLayers Refill list of available layers and correct ability selection of required layer
//! \param required     Layer required selected
//! \return             Available selected layer
//!
SdLayer *SdPropBarLay::refillLayers(SdLayer *required)
  {
  mLayer->clear();
  //fill new layers list
  bool available = false;
  SdLayer *first = nullptr;
  SdEnvir::instance()->layerForEachConst( mEditObjectClass, [this,required,&available,&first] ( SdLayer *p ) -> bool {
    if( p->isEdited() && p->stratum().isIntersect(mStratum) ) {
      //Append layer p into available layer list
      mLayer->addItem( p->name(), QVariant( p->id() ) );

      //Compare appended layer to required
      if( p == required )
        //If equal then we mark required layer as available to select
        available = true;

      //If first available layer not yet assigned, then we assign it
      if( first == nullptr )
        first = p;
      }
    return true;
    } );

  if( !available ) {
    //When required layer is not available to selection, then we
    //change required to first available layer of list
    required = first;
    }

  return required;
  }



