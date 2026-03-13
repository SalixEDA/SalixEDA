/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Base for objects with graphical presentation
*/

#include "SdGraph.h"
#include "SdSelector.h"
#include "SvLib/SvTime2x.h"

QMap<int,int> SdGraph::mGroupMap;

SdGraph::SdGraph() :
  SdObject(),
  mGroupId(0),
  mSelector(nullptr)
  {

  }




//!
//! \brief groupIdSet Sets new group id for object, 0 for remove group id
//! \param newGroupId New group id or 0 for remove group id
//! \param undo       Undo object to save previous group id
//!
void SdGraph::groupIdSet(int newGroupId, SdUndo *undo)
  {
  if( mGroupId != newGroupId ) {
    if( undo != nullptr )
      undo->prop( &mGroupId );
    mGroupId = newGroupId;
    }
  }



//!
//! \brief groupIdNew Returns new group id
//! \return           New unical group id
//!
int SdGraph::groupIdNew()
  {
  static int groupIdCount = 0;
  if( groupIdCount + 1000 < SvTime2x::current() )
    groupIdCount = SvTime2x::current();
  return groupIdCount++;
  }



void SdGraph::saveState(SdUndo *undo)
  {
  Q_UNUSED(undo)
  }




void SdGraph::moveComplete(SdPoint grid, SdUndo *undo)
  {
  Q_UNUSED(undo)
  Q_UNUSED(grid)
  }



void SdGraph::transform(const QTransform &map, SdPvAngle angle)
  {
  Q_UNUSED(map)
  Q_UNUSED(angle)
  }




void SdGraph::setProp(SdPropSelected &prop)
  {
  Q_UNUSED(prop)
  }

void SdGraph::getProp(SdPropSelected &prop)
  {
  Q_UNUSED(prop)
  }

void SdGraph::setText(int index, QString sour, SdPropText &prop, QWidget *parent)
  {
  Q_UNUSED(index)
  Q_UNUSED(sour)
  Q_UNUSED(prop)
  Q_UNUSED(parent)
  }

void SdGraph::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  Q_UNUSED(p)
  Q_UNUSED(selector)
  }

void SdGraph::selectByRect(const SdRect &r, SdSelector *selector)
  {
  Q_UNUSED(r)
  Q_UNUSED(selector)
  }

void SdGraph::select(SdSelector *selector)
  {
  if( selector != nullptr )
    selector->insert( this );
  }



void SdGraph::prepareMove(SdUndo *undo)
  {
  Q_UNUSED(undo)
  }






void SdGraph::setLayerUsage()
  {
  }



void SdGraph::draw(SdContext *dc)
  {
  Q_UNUSED(dc)
  }



//!
//! \brief drawExceptText Draws object in 2d space with exception textual component with textId
//! \param dc             Display context where drawing doing
//! \param textId         Text id which must not be drawn
//!
void SdGraph::drawExceptText(SdContext *dc, int textId)
  {
  Q_UNUSED(dc)
  Q_UNUSED(textId)
  }




void SdGraph::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  Q_UNUSED(f)
  }




//!
//! \brief accumHoles Accum holes description into faceList
//! \param model      Model which accumulate coord vertexes
//! \param faceList   Face list for holding holes (single face for single hole)
//! \param stratum    Stratum for layers
//! \param map        Map for holes conversion
//!
void SdGraph::accumHoles(Sd3drModel &model, Sd3drFaceList &faceList, SdPvStratum stratum, const QMatrix4x4 &map) const
  {
  Q_UNUSED(model)
  Q_UNUSED(faceList)
  Q_UNUSED(stratum)
  Q_UNUSED(map)
  }



int SdGraph::behindCursor(SdPoint p)
  {
  Q_UNUSED(p)
  return 0;
  }



int SdGraph::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  Q_UNUSED(p)
  Q_UNUSED(org)
  Q_UNUSED(dest)
  Q_UNUSED(prop)
  return 0;
  }

bool SdGraph::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  Q_UNUSED(p)
  Q_UNUSED(info)
  Q_UNUSED(extInfo)
  return false;
  }



//Find snap point on object
void SdGraph::snapPoint(SdSnapInfo *snap)
  {
  Q_UNUSED(snap)
  }



//!
//! \brief cloneFrom Clone contents object except mParent field. This must be overrided in all subclasses to
//!                  generate right copy of object
//!                  Cloned object has no parent
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraph::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdObject::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraph> graph(src);
  if( graph.isValid() && graph->mGroupId ) {
    //If current map not contains source group id then we append new association
    if( !mGroupMap.contains( graph->mGroupId ) )
      mGroupMap.insert( graph->mGroupId, groupIdNew() );
    mGroupId = mGroupMap.value( graph->mGroupId );
    }
  else mGroupId = 0;
  }



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdGraph::json(SdJsonWriter &js) const
  {
  js.jsonInt( "GroupId", mGroupId );
  SdObject::json( js );
  }



//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdGraph::json(const SdJsonReader &js)
  {
  js.jsonInt( "GroupId", mGroupId, 0 );
  SdObject::json( js );
  }



