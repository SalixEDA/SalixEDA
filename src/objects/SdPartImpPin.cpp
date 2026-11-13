/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Part implementation pin
*/
#include "SdPartImpPin.h"

#include "SdGraphPartPin.h"
#include "SdGraphSymPinsMap.h"
#include "SdPItemPlate.h"
#include "SdContext.h"
#include "SdEnvir.h"
#include "SdJsonIO.h"


SdPartImpPin::SdPartImpPin() :
  mPin(nullptr),       //Original pin
  mSection(nullptr)    //Schematic section where pin is located
  {
  }




//!
//! \brief operator = Assign pin state from another implementation pin
//! \param pin        Source pin to copy from
//!
void SdPartImpPin::operator =(const SdPartImpPin &pin)
  {
  mPin       = pin.mPin;
  mSection   = pin.mSection;
  mPinName   = pin.mPinName;   //Part pin name
  mPosition  = pin.mPosition;  //Pin position in plate context
  mStratum   = pin.mStratum;   //Pin stratum
  }






//!
//! \brief draw       Draw pin pad and pin graphics for the specified stratum
//! \param dc         Drawing context
//! \param plate      Plate which provides pad geometry and association
//! \param stratum    Stratum filter for drawing
//!
//! Draws the pad using plate pad association and then draws pin connection
//! point, number and name graphics.
//!
void SdPartImpPin::draw(SdContext *dc, SdPItemPlate *plate, SdPvStratum stratum ) const
  {
  //Draw pin pad
  plate->drawPad( dc, mPin->getPinOrigin(), mPin->getPinType(), mStratum & stratum );
  //Draw pin connection and also pin name and number
  drawWithoutPad( dc );
  }





//!
//! \brief drawWithoutPad Draw pin graphics without pad geometry
//! \param dc             Drawing context
//!
//! Draws only the pin connection point, pin number and pin name using the
//! original part pin graphics.
//!
void SdPartImpPin::drawWithoutPad(SdContext *dc) const
  {
  //Draw standard pin (pin connection point, pin number and pin name)
  mPin->drawImp( dc, mPinName, isConnected() );
  }






//!
//! \brief drawPad     Draw only the pin pad for the specified stratum
//! \param dc          Drawing context
//! \param plate       Plate which provides pad geometry and association
//! \param stratum     Stratum filter for drawing
//! \param highliteNet Net name to highlight, or empty string for normal drawing
//!
//! If \a highliteNet matches the pin net, the pad is drawn with the trace
//! highlight color. Used when pads and other pin graphics are rendered separately.
//!
void SdPartImpPin::drawPad(SdContext *dc, SdPItemPlate *plate, SdPvStratum stratum, const QString highliteNet ) const
  {
  //Draw pin pad
  if( !highliteNet.isEmpty() && getNetName() == highliteNet )
    dc->setOverColor( SdEnvir::instance()->getSysColor(scTraseNet) );
  plate->drawPad( dc, mPin->getPinOrigin(), mPin->getPinType(), mStratum & stratum );
//  plate->drawPad( dc, mPosition, mPin->getPinType(), mStratum.stratum() & stratum );
  dc->resetOverColor();
  }




//!
//! \brief isConnected Check whether this pin is connected to a schematic net
//! \return            true if the pin is linked to a schematic section and that section reports the pin as connected
//!
bool SdPartImpPin::isConnected() const
  {
  return mSection != nullptr && mSection->isPinConnected(mPinName);
  }




//!
//! \brief getNetName Get net name assigned to this pin
//! \return           Net name from the linked schematic section, or empty string if not connected
//!
QString SdPartImpPin::getNetName() const
  {
  if( mSection != nullptr )
    return mSection->pinNetName( mPinName );
  return QString();
  }





//!
//! \brief accumUsedPin Register pad types used by this pin
//! \param map          Map of used pad types to pad descriptions
//!
//! Ensures that the pad type required by this pin is present in the pad map
//! so that pad descriptions can be resolved for plate operations.
//!
void SdPartImpPin::accumUsedPin(SdPadMap &map) const
  {
  if( !map.contains(mPin->getPinType()) )
    //Append new pin-to-pad association with default pad
    map.insert( mPin->getPinType(), SdEnvir::instance()->getPad(mPin->getPinType()) );
  }




//!
//! \brief accumNetSegments Append net connectivity segments for this pin
//! \param traced           Traced object that owns the segment
//! \param netContainer     Container receiving net segments
//!
//! For a connected pin this adds segment(s) representing its electrical
//! connection point(s) to the net container. Currently a single zero-length
//! segment is produced at the pin position; for composite pins this is the
//! place to add segments for all physical pad forms.
//!
void SdPartImpPin::accumNetSegments(SdGraphTraced *traced, SdPlateNetContainer *netContainer) const
  {
  if( isConnected() )
    netContainer->addNetSegment( traced, getNetName(), mStratum, mPosition, mPosition );
  }




//!
//! \brief accumBarriers Accumulate tracing barriers produced by this pin pad
//! \param plate         Plate which provides pad geometry and net rules
//! \param dest          Barrier list to append to
//! \param stratum       Stratum filter for barriers
//! \param ruleId        Rule identifier that defines required clearance
//! \param clearance     Base clearance value
//! \param halfWidth     Half of road width used for tracing geometry
//! \param t             Transform from part-local to plate coordinates
//!
//! Builds barrier polygon(s) for the pin pad in plate coordinates and appends
//! them to \a dest. Clearance and road half-width are applied according to
//! net/rule requirements to support tracing and clearance checks.
//!
void SdPartImpPin::accumBarriers(SdPItemPlate *plate, SdBarrierList &dest, SdPvStratum stratum, SdRuleId ruleId, int clearance, int halfWidth, QTransform t ) const
  {

  //Compare on stratum
  if( mStratum.isIntersect(stratum) ) {
    if( ruleId >= ruleLast )
      clearance = 0;
    else
      clearance = qMax( clearance, plate->ruleForNet( getNetName(), ruleId ) );
    SdBarrier bar;
    bar.mNetName = getNetName();
    bar.mPolygon = t.map( plate->getPadPolygon( mPin->getPinOrigin(), mPin->getPinType(), clearance + halfWidth) );
    dest.append( bar );
    }
  }





//!
//! \brief accumWindows Accumulate polygon windows produced by this pin pad
//! \param plate        Plate which provides pad window geometry
//! \param dest         Polygon window list to append to
//! \param stratum      Stratum filter for windows
//! \param gap          Gap around pad geometry
//! \param netName      Net name of the polygon area being processed
//! \param t            Transform from part-local to plate coordinates
//!
//! Adds pad window geometry for copper pours/polygons when the pin belongs to
//! a different net than \a netName and matches the requested stratum.
//!
void SdPartImpPin::accumWindows(SdPItemPlate *plate, SdPolyWindowList &dest, SdPvStratum stratum, int gap, const QString netName, const QTransform &t) const
  {
  //Compare on stratum
  if( mStratum.isIntersect(stratum) && netName != getNetName() )
    //Stratum matched and net name other
    plate->appendPadWindow( dest, mPin->getPinOrigin(), mPin->getPinType(), gap, t );
  }




//!
//! \brief accumHoles Accumulate hole descriptions into face list
//! \param plate      Plate through which pads are supported
//! \param model      Model which accumulates coordinate vertexes
//! \param faceList   Face list for holding holes (single face for single hole)
//! \param stratum    Stratum for layers
//! \param map        Map for hole conversion
//!
//! Appends 3D hole geometry generated by the pin pad (for example through-hole
//! or mounting holes defined by the pad) into the model and face list.
//!
void SdPartImpPin::accumHoles(SdPItemPlate *plate, Sd3drModel &model, Sd3drFaceList &faceList, SdPvStratum stratum, const QMatrix4x4 &map) const
  {
  plate->appendPadHoles( mPin->getPinOrigin(), mPin->getPinType(), model, faceList, stratum, map );
  }




//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPartImpPin::json(SdJsonWriter &js) const
  {
  //Original pin
  js.jsonObjectPtr( QStringLiteral("Pin"), mPin );

  //Section
  js.jsonObjectPtr( QStringLiteral("Section"), mSection );

  //Part pin name
  js.jsonString( QStringLiteral("PinNam"), mPinName );

  //Pin position in plate context
  js.jsonPoint( QStringLiteral("Pos"), mPosition );
  mStratum.json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdPartImpPin::json(const SdJsonReader &js)
  {
  //Original pin
  js.jsonObjectPtr( QStringLiteral("Pin"), mPin );

  //Section
  js.jsonObjectPtr( QStringLiteral("Section"), mSection );

  //Part pin name
  js.jsonString( QStringLiteral("PinNam"), mPinName );

  //Pin position in plate context
  js.jsonPoint( QStringLiteral("Pos"), mPosition );
  mStratum.json( js );
  }








QString SdPartImpPin::fromJson(SdJsonReaderProperty *map, const QJsonObject obj)
  {
  SdJsonReader js( obj, map );
  json( js );
  return obj.value( QStringLiteral("PinNum") ).toString();
  }
