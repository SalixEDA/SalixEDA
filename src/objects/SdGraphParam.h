/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdGraphParam - base class for graphical child objects with a local parameter table
*/
#ifndef SDGRAPHPARAM_H
#define SDGRAPHPARAM_H

#include "SdGraph.h"
#include "SdParamMap.h"


class SdGraphParam : public SdGraph, public SdParamMap
  {
  public:
    SdGraphParam();
    SdGraphParam( const SdStringMap &param );

    //!
    //! \brief paramSet      Set parameter value in the local table without undo support
    //! \param key           Key of the parameter
    //! \param val           Value to assign to the parameter
    //!
    void               paramSet( const QString key, const QString val ) { SdParamMap::paramSet( key, val, nullptr ); }

    using SdParamMap::paramSet;

    //!
    //! \brief setDirtyFlagForParamMap Mark the owner state as modified after parameter changes
    //!
    virtual void       setDirtyFlagForParamMap() override { setProjectDirtyFlag(); }

    // SdObject interface
  public:
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void       cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void       json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void       json( const SdJsonReader &js ) override;
  };

#endif // SDGRAPHPARAM_H
