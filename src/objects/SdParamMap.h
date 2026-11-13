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
#ifndef SDPARAMMAP_H
#define SDPARAMMAP_H

#include "library/SdStringMap.h"

class SdUndo;

class SdParamMap
  {
  protected:
    SdStringMap mParamTable; //!< Local parameter table storing key-value pairs

  public:
    //!
    //! \brief paramContains Test if a parameter is present in the local table
    //! \param key           Key of the parameter
    //! \return              true if a parameter with the given key is present in the local table
    //!
    virtual bool       paramContains( const QString key ) const { return mParamTable.contains(key); }

    //!
    //! \brief paramGet      Get parameter value from the local table
    //! \param key           Key of the parameter
    //! \return              Parameter value, or an empty string if the key is not present
    //!
    virtual QString    paramGet( const QString key ) const { return mParamTable.value(key); }

    //!
    //! \brief paramSet      Set parameter value in the local table
    //! \param key           Key of the parameter
    //! \param val           Value to assign to the parameter
    //! \param undo          Undo object to record the change, or nullptr if undo is not required
    //!
    virtual void       paramSet( const QString key, QString val, SdUndo *undo );

    //!
    //! \brief paramDelete   Delete parameter from the local table
    //! \param key           Key of the parameter
    //! \param undo          Undo object to record the change, or nullptr if undo is not required
    //!
    virtual void       paramDelete( const QString key, SdUndo *undo );

    //!
    //! \brief paramTable    Get the full local parameter table
    //! \return              Constant reference to the local parameter table
    //!
    const SdStringMap &paramTable() const { return mParamTable; }

    //!
    //! \brief paramTableSet Replace the full local parameter table
    //! \param map           New parameter table
    //! \param undo          Undo object to record the change, or nullptr if undo is not required
    //!
    virtual void       paramTableSet( const SdStringMap &map, SdUndo *undo );

    //!
    //! \brief setDirtyFlagForParamMap Mark the owner state as modified after parameter changes
    //!
    virtual void       setDirtyFlagForParamMap() = 0;
  };

#endif // SDPARAMMAP_H
