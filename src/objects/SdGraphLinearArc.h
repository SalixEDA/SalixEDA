/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Graphical object Arc.
*/
#ifndef SDGRAPHLINEARARC_H
#define SDGRAPHLINEARARC_H

#include "SdGraphLinear.h"
#include "SdPoint.h"
#include "SdRect.h"

#define SD_TYPE_ARC "Arc"

class SdGraphLinearArc : public SdGraphLinear
  {
    SdPoint mCenter;  //!< Center of arc's circle
    SdPoint mStart;   //!< Start point of arc.
    SdPoint mMiddle;  //!< Middle point of arc. Define direction of arc
    SdPoint mStop;    //!< Stop point of arc
  public:
    SdGraphLinearArc();
    SdGraphLinearArc( SdPoint center, SdPoint start, SdPoint stop, const SdPropLine &propLine );


    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral( SD_TYPE_ARC ); }
    virtual quint64 getClass() const override { return dctLines; }
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void    cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief isClone Test if object src is clone of current object or viseversa
    //! \param src     Source object
    //! \return        true when object src is clone of current object or viseversa
    //!
    virtual bool    isClone( const SdObject *src ) const override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void    json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void    json( const SdJsonReader &js ) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    transform( const QTransform &map, SdPvAngle ) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    //Find snap point on object
    virtual void    snapPoint(SdSnapInfo *snap) override;

    friend class SdPasCadImport;
  };

#endif // SDGRAPHLINEARARC_H
