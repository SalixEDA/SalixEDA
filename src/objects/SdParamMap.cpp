/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdParamMap - base class providing a local parameter table (key-value string map)
  with undo support. Derived classes must implement setDirtyFlagForParamMap()
  to mark the owner state as modified when parameters change.

*/
#include "SdParamMap.h"
#include "SdUndo.h"



//!
//! \brief paramSet      Set parameter value in the local table
//! \param key           Key of the parameter
//! \param val           Value to assign to the parameter
//! \param undo          Undo object to record the change, or nullptr if undo is not required
//!
void SdParamMap::paramSet(const QString key, QString val, SdUndo *undo)
  {
  if( undo != nullptr )
    undo->stringMapItem( &mParamTable, key );
  mParamTable.insert( key, val );
  }





//!
//! \brief paramDelete   Delete parameter from the local table
//! \param key           Key of the parameter
//! \param undo          Undo object to record the change, or nullptr if undo is not required
//!
void SdParamMap::paramDelete(const QString key, SdUndo *undo)
  {
  if( undo != nullptr )
    undo->stringMapItem( &mParamTable, key );
  mParamTable.remove( key );
  }





//!
//! \brief paramTableSet Replace the full local parameter table
//! \param map           New parameter table
//! \param undo          Undo object to record the change, or nullptr if undo is not required
//!
void SdParamMap::paramTableSet(const SdStringMap &map, SdUndo *undo)
  {
  if( undo != nullptr )
    undo->prop( &mParamTable ).post( [this]() { setDirtyFlagForParamMap(); } );
  mParamTable = map;
  setDirtyFlagForParamMap();
  }
