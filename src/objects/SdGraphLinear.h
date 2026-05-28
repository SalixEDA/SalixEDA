/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Graphical object with linear type (line, rect, circle and so).
*/
#ifndef SDOBJECTGRAPHLINEAR_H
#define SDOBJECTGRAPHLINEAR_H

#include "SdGraph.h"
#include "SdPropLine.h"

class SdGraphLinear : public SdGraph
  {
  protected:
    SdPropLine mProp; //!< Linear graph object props
  public:
    SdGraphLinear();
    SdGraphLinear( const SdPropLine &propLine );

    //

    //!
    //! \brief isMatchLayer Compare if is layer match to object layer
    //! \param layer        Tested layer
    //! \return             true if tested layer matched to object layer
    //!
    bool isMatchLayer( SdLayer *layer ) const { return mProp.mLayer.layer() == layer; }

    //!
    //! \brief isMatchId Compare unical id of object layer with given id
    //! \param id        Id for match
    //! \return          true if id of object layer start with given id
    //!
    bool isMatchId( const QString &id ) const { return mProp.mLayer.layer()->id().startsWith( id ); }

    //!
    //! \brief isMatchAny Compare unical id object layer with any of id from given list
    //! \param idList     List of id's
    //! \return           true if id of object layer start with any of id from given list
    //!
    bool isMatchAny( const QStringList &idList ) const
      {
      for( const auto &id : idList )
        if( isMatchId( id ) ) return true;
      return false;
      }



    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief isClone Test if object src is clone of current object or viseversa
    //! \param src     Source object
    //! \return        true when object src is clone of current object or viseversa
    //!
    virtual bool isClone( const SdObject *src ) const override;

    virtual void json( SdJsonWriter &js) const override;
    virtual void json(const SdJsonReader &js) override;

    //Properties service [Изменение свойствами]
    virtual void setProp(SdPropSelected &prop) override;
    virtual void getProp(SdPropSelected &prop) override;
    virtual bool layerReplace( const QStringList &pairs ) override;

    //Проверить доступность объекта
    virtual bool isVisible() const override;

    virtual void setLayerUsage() override;

    friend class SdPasCadImport;
  };

#endif // SDOBJECTGRAPHLINEAR_H
