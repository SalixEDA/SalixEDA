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
#ifndef SDPARTIMPPIN_H
#define SDPARTIMPPIN_H

#include "SdPoint.h"
#include "SdPvStratum.h"
#include "SdPadAssociation.h"
#include "SdBarrier.h"
#include "SdPlateNetList.h"

#include <QMap>
#include <QMatrix4x4>
#include <QString>

class SdContext;
class SdPItemPlate;
class SdGraphPartPin;
class SdGraphSymPinsMap;
class SdGraphTraced;
class SdPlateNetContainer;
class Sd3drModel;
class SdJsonWriter;
class SdJsonReader;
struct SdJsonReaderProperty;

//Part implementation pin [Ножка вхождения корпуса]
struct SdPartImpPin {
  SdGraphPartPin    *mPin;       //!< Original pin
  SdGraphSymPinsMap *mSection;   //!< Schematic section where pin located
  QString            mPinName;   //!< Part pin name
  SdPoint            mPosition;  //!< Pin position in plate context
  SdPvStratum        mStratum;   //!< Pin stratum

  SdPartImpPin();

  //!
  //! \brief operator = Assign pin state from another implementation pin
  //! \param pin        Source pin to copy from
  //!
  void        operator = ( const SdPartImpPin &pin );

  //!
  //! \brief draw       Draw pin pad and pin graphics for the specified stratum
  //! \param dc         Drawing context
  //! \param plate      Plate which provides pad geometry and association
  //! \param stratum    Stratum filter for drawing
  //!
  //! Draws the pad using plate pad association and then draws pin connection
  //! point, number and name graphics.
  //!
  void        draw( SdContext *dc, SdPItemPlate *plate, SdPvStratum stratum ) const;

  //!
  //! \brief drawWithoutPad Draw pin graphics without pad geometry
  //! \param dc             Drawing context
  //!
  //! Draws only the pin connection point, pin number and pin name using the
  //! original part pin graphics.
  //!
  void        drawWithoutPad( SdContext *dc ) const;

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
  void        drawPad( SdContext *dc, SdPItemPlate *plate, SdPvStratum stratum, const QString highliteNet ) const;

  //!
  //! \brief isConnected Check whether this pin is connected to a schematic net
  //! \return            true if the pin is linked to a schematic section and that section reports the pin as connected
  //!
  bool        isConnected() const;

  //!
  //! \brief getNetName Get net name assigned to this pin
  //! \return           Net name from the linked schematic section, or empty string if not connected
  //!
  QString     getNetName() const;

  //!
  //! \brief accumUsedPin Register pad types used by this pin
  //! \param map          Map of used pad types to pad descriptions
  //!
  //! Ensures that the pad type required by this pin is present in the pad map
  //! so that pad descriptions can be resolved for plate operations.
  //!
  void        accumUsedPin( SdPadMap &map ) const;

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
  void        accumNetSegments( SdGraphTraced *traced, SdPlateNetContainer *netContainer ) const;

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
  void        accumBarriers( SdPItemPlate *plate, SdBarrierList &dest, SdPvStratum stratum, SdRuleId ruleId, int clearance, int halfWidth, QTransform t ) const;

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
  void        accumWindows( SdPItemPlate *plate, SdPolyWindowList &dest, SdPvStratum stratum, int gap, const QString netName, const QTransform &t ) const;

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
  void        accumHoles( SdPItemPlate *plate, Sd3drModel &model, Sd3drFaceList &faceList, SdPvStratum stratum, const QMatrix4x4 &map ) const;

  //!
  //! \brief json Overloaded function to write object content into json writer
  //! \param js   Json writer
  //!
  void        json( SdJsonWriter &js ) const;

  //!
  //! \brief json Overloaded function to read object content from json reader
  //! \param js   Json reader
  //!
  void        json( const SdJsonReader &js );

  //!
  //! \brief fromJson Read pin data from a json object using a property map
  //! \param map        Property map used for resolving object references
  //! \param obj        Json object containing pin data
  //! \return           Pin number extracted from json data
  //!
  QString     fromJson( SdJsonReaderProperty *map, const QJsonObject obj );
  };

typedef QMap<QString,SdPartImpPin> SdPartImpPinTable;

#endif // SDPARTIMPPIN_H
