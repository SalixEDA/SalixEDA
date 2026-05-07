#include "SdGraphNetPinsList.h"
#include "SdSelector.h"
#include "SdContext.h"
#include "SdGraphPartImp.h"

#include <QObject>




//!
//! \brief pinList Returns pin list
//! \return        Pin list
//!
SdNetPinRefList SdGraphNetPinsList::pinList() const
  {
  //From map build list and return
  SdNetPinRefList list;
  for( auto it = mPinRefMap.cbegin(); it != mPinRefMap.cend(); ++it )
    list.append( it.value() );
  return list;
  }





//!
//! \brief pinListSet Set new pin list
//! \param netName    New net name
//! \param list       New pin list
//! \param undo       Undo storage
//!
void SdGraphNetPinsList::pinListSet(const QString &netName, const SdNetPinRefList &list, SdUndo *undo)
  {
  //Save previous pin ref map
  if( undo )
    undo->prop( &mNetName, &mPinRefMap ).pred( [this]() { disconnectAll(); } ).post( [this]() { connectAll(); } );

  //Disconnect it
  disconnectAll();

  //Build new pin ref map on base given list
  mPinRefMap.clear();
  for( const auto &pinRef : std::as_const(list) ) {
    mPinRefMap.insert( QString("Pn%1").arg(mPinNameIndex++), pinRef );
    }

  mNetName = netName;

  //Connect to parts and build visual representation
  connectAll();
  }






//!
//! \brief unLinkPart Unlink part impelement from symbol implement
//! \param partImp    Part implement
//! \param undo       Undo operation
//!
void SdGraphNetPinsList::unLinkPart(SdGraphPartImp *partImp, SdUndo *undo)
  {
  //Save previous pin ref map
  if( undo )
    undo->prop( &mPinRefMap ).pred( [this]() { disconnectAll(); } ).post( [this]() { connectAll(); } );

  //Collect pin names for unlinked part imp
  QStringList names;
  for( auto it = mPinRefMap.cbegin(); it != mPinRefMap.cend(); ++it )
    //If pin apply to given part imp, then add its name to list
    if( it.value().mPartImp == partImp )
      names.append( it.key() );

  //For each name in collection we unconnect from part imp and remove pin from map
  for( auto const &pinName : std::as_const(names) ) {
    //Unconnect pin in part imp
    mPinRefMap.value( pinName ).mPartImp->partPinLink( mPinRefMap.value( pinName ).mPinNumber, this, pinName, false );
    //Remove pin from table
    mPinRefMap.remove( pinName );
    }
  }





void SdGraphNetPinsList::detach(SdUndo *undo)
  {
  //Save previous connections
  if( undo )
    undo->prop( &mPinRefMap ).pred( [this]() { disconnectAll(); } ).post( [this]() { connectAll(); } );

  //Disconnect from part imp
  disconnectAll();

  //Remove all pins from map
  mPinRefMap.clear();
  }



void SdGraphNetPinsList::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  //We clone only net name and text properties
  //PinRefMap not copied because on copy will be others part imps
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
  js.jsonString( "Text", mText );
  js.jsonInt( "PinNameIndex", mPinNameIndex );
  js.jsonMap( js, QStringLiteral("PinRefMap"), mPinRefMap );
  mVisual.json( "Visual", js );
  }


void SdGraphNetPinsList::json(const SdJsonReader &js)
  {
  js.jsonString( "NetName", mNetName );
  js.jsonString( "Text", mText );
  js.jsonInt( "PinNameIndex", mPinNameIndex );
  js.jsonMap( js, QStringLiteral("PinRefMap"), mPinRefMap );
  mVisual.json( "Visual", js );
  }




void SdGraphNetPinsList::saveState(SdUndo *undo)
  {
  //We save state only for visual prop
  if( undo != nullptr )
    undo->prop( &mVisual );
  }




void SdGraphNetPinsList::transform(const QTransform &map, SdPvAngle angle)
  {
  //Transform for visual prop
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




void SdGraphNetPinsList::disconnectAll()
  {
  for( auto it = mPinRefMap.cbegin(); it != mPinRefMap.cend(); ++it )
    if( it.value().mPartImp != nullptr ) {
      it.value().mPartImp->partPinLink( it.value().mPinNumber, this, it.key(), false );
      }
  }



void SdGraphNetPinsList::connectAll()
  {
  mText = QObject::tr("Net \"%1\" connect to pins: ").arg(mNetName);
  bool addComma = false;
  for( auto it = mPinRefMap.cbegin(); it != mPinRefMap.cend(); ++it )
    if( it.value().mPartImp != nullptr ) {
      //Fact link pin
      it.value().mPartImp->partPinLink( it.value().mPinNumber, this, it.key(), true );

      //Build textual representation
      if( addComma )
        mText += QStringLiteral(", ");
      addComma = true;
      mText += it.value().mPartImp->ident() + QStringLiteral(":") + it.value().mPinNumber;
      }
  }




