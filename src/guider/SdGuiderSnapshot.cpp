/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdGuiderSnapshot is used to save the state of the graphic editor window of the project object.
*/
#include "guider/SdGuiderSnapshot.h"


//!
//! \brief json Function to write object into json writer
//! \param js   Json writer
//!
void SdGuiderSnapshot::json(SdJsonWriter &js) const
  {
  js.jsonString( "ProjectName", mProjectName );
  js.jsonString( "ObjectName",  mObjectName );
  mGrid.json( "Grid-", js );
  mOrigin.json( "Origin-", js );
  js.jsonDouble( "Scale", mScale );
  js.jsonBool( "is3d", mIs3d );
  }



//!
//! \brief json Function to read object from json reader
//! \param js   Json reader
//!
void SdGuiderSnapshot::json(const SdJsonReader &js)
  {
  js.jsonString( "ProjectName", mProjectName );
  js.jsonString( "ObjectName",  mObjectName );
  mGrid.json( "Grid-", js );
  mOrigin.json( "Origin-", js );
  js.jsonDouble( "Scale", mScale );
  js.jsonBool( "is3d", mIs3d );
  }
