#include "SdGraphNetPinsList.h"
#include "SdSelector.h"
#include "SdContext.h"
#include "SdGraphPartImp.h"






SdNetPinRefList SdGraphNetPinsList::pinList() const
  {
  SdNetPinRefList list;
  for( auto it = mPinRefMap.cbegin(); it != mPinRefMap.cend(); ++it )
    list.append( it.value() );
  return list;
  }





void SdGraphNetPinsList::pinListSet(const SdNetPinRefList &list, SdUndo *undo)
  {
  if( undo )
    undo->prop( &mPinRefMap ).pred( [this]() { disconnectAll(); } ).post( [this]() { connectAll(); } );

  disconnectAll();

  mPinRefMap.clear();
  for( const auto &pinRef : std::as_const(list) ) {
    mPinRefMap.insert( QString("Pn%1").arg(mPinNameIndex++), pinRef );
    }

  connectAll();
  }






void SdGraphNetPinsList::unLinkPart(SdGraphPartImp *partImp, SdUndo *undo)
  {
  if( undo )
    undo->prop( &mPinRefMap ).pred( [this]() { disconnectAll(); } ).post( [this]() { connectAll(); } );

  QStringList names;
  for( auto it = mPinRefMap.cbegin(); it != mPinRefMap.cend(); ++it )
    if( it.value().mPartImp == partImp ) names.append( it.key() );

  for( auto const &pinName : std::as_const(names) ) {
    //Unconnect pin in part imp
    mPinRefMap.value( pinName ).mPartImp->partPinLink( mPinRefMap.value( pinName ).mPinNumber, this, pinName, false );
    //Remove pin from table
    mPinRefMap.remove( pinName );
    }
  }





void SdGraphNetPinsList::detach(SdUndo *undo)
  {
  if( undo )
    undo->prop( &mPinRefMap ).pred( [this]() { disconnectAll(); } ).post( [this]() { connectAll(); } );

  disconnectAll();
  mPinRefMap.clear();
  }



void SdGraphNetPinsList::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraph::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphNetPinsList> netPins(src);
  Q_ASSERT_X( netPins.isValid(), "SdGraphNetPinsList::cloneFrom", "Cloned not SdGraphNetPinsList" );
  mNetName      = netPins->mNetName;
  mPinNameIndex = 0;
  mVisual       = netPins->mVisual;
  }



void SdGraphNetPinsList::json(SdJsonWriter &js) const
  {
  js.jsonString( "NetName", mNetName );
  js.jsonInt( "PinNameIndex", mPinNameIndex );
  js.jsonMap( js, QStringLiteral("PinRefMap"), mPinRefMap );
  mVisual.json( "Visual", js );
  }


void SdGraphNetPinsList::json(const SdJsonReader &js)
  {
  js.jsonString( "NetName", mNetName );
  js.jsonInt( "PinNameIndex", mPinNameIndex );
  js.jsonMap( js, QStringLiteral("PinRefMap"), mPinRefMap );
  mVisual.json( "Visual", js );
  }




void SdGraphNetPinsList::saveState(SdUndo *undo)
  {
  if( undo != nullptr )
    undo->prop( &mNetName, &mPinRefMap, &mVisual ).pred( [this]() { disconnectAll(); } ).post( [this]() { connectAll(); } );
  }




void SdGraphNetPinsList::transform(const QTransform &map, SdPvAngle angle)
  {
  mVisual.mOrigin = map.map(mVisual.mOrigin);
  mOverRect = map.mapRect(mOverRect);
  mVisual.mProp.mDir += angle;
  }





void SdGraphNetPinsList::setProp(SdPropSelected &prop)
  {
  prop.mTextProp.store( mVisual.mProp );
  }




void SdGraphNetPinsList::getProp(SdPropSelected &prop)
  {
  prop.mTextProp.append( mVisual.mProp );
  prop.mFilledPropMask |= spsTextProp;
  }





void SdGraphNetPinsList::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mVisual.mProp.mLayer.isEdited() ) {
    if( !getSelector() && mOverRect.isPointInside(p) ) {
      selector->insert( this );
      }
    }
  }




void SdGraphNetPinsList::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mVisual.mProp.mLayer.isEdited() ) {
    if( !getSelector() && r.isAccross( mOverRect ) ) {
      selector->insert( this );
      }
    }
  }



void SdGraphNetPinsList::setLayerUsage()
  {
  mVisual.mProp.mLayer.setLayerUsage();
  }



bool SdGraphNetPinsList::isVisible() const
  {
  return mVisual.mProp.mLayer.isVisible();
  }



void SdGraphNetPinsList::draw(SdContext *dc)
  {
  //Draw as simple text
  dc->text( mVisual.mOrigin, mOverRect, mText, mVisual.mProp );
  }




int SdGraphNetPinsList::behindCursor(SdPoint p)
  {
  if( mVisual.mProp.mLayer.isEdited() ) {
    if( mOverRect.isPointInside(p) )
      return getSelector() ? ELEM_SEL : ELEM_UNSEL;
    }
  return 0;
  }




