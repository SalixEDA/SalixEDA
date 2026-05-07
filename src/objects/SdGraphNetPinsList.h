/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdGraphNetPinsList - textual representation of net pin list.
  Each object is for single net, but multiple part and pin. So we have
  netName and pins map. Map is pinName-pin association. PinName we
  generate internal.
*/
#ifndef SDGRAPHNETPINSLIST_H
#define SDGRAPHNETPINSLIST_H

#include "SdGraphSymPinsMap.h"
#include "SdTextImplement.h"

#include <QMap>
#include <QList>

#define SD_TYPE_NET_PINS_LIST "NetPinsList"


class SdGraphPartImp;

struct SdNetPinRef
  {
    SdGraphPartImp *mPartImp;   //!< Part implement of pin
    QString         mPinNumber; //!< Pin number

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

  };

using SdNetPinRefMap = QMap<QString,SdNetPinRef>;

using SdNetPinRefList = QList<SdNetPinRef>;



class SdGraphNetPinsList : public SdGraphSymPinsMap
  {
    QString         mNetName;      //!< Net, to which pins are connected
    int             mPinNameIndex; //!< Index for building unical pin name
    SdNetPinRefMap  mPinRefMap;    //!< Pin list for net
    SdTextImplement mVisual;       //!< Net pin list text properties and position
    QString         mText;         //!< Textual representation of list

    //Not saved
    SdRect          mOverRect;     //!< Rectangle around text representation of list
  public:
    SdGraphNetPinsList() : SdGraphSymPinsMap(), mPinNameIndex(0) {}

    //!
    //! \brief netName Returns net name for netPinsList
    //! \return        Net name for netPinsList
    //!
    QString           netName() const { return mNetName; }

    //!
    //! \brief pinList Returns pin list
    //! \return        Pin list
    //!
    SdNetPinRefList   pinList() const;

    //!
    //! \brief pinListSet Set new pin list
    //! \param netName    New net name
    //! \param list       New pin list
    //! \param undo       Undo storage
    //!
    void              pinListSet( const QString &netName, const SdNetPinRefList &list, SdUndo *undo );

    //!
    //! \brief isPinConnected Returns the connection status of a pin.
    //! \param pinName        Name of the pin to be checked.
    //! \return               true if a net is connected to the pin, false otherwise.
    //!
    virtual bool      isPinConnected( const QString pinName ) const override { return mPinRefMap.contains(pinName); }

    //!
    //! \brief pinNetName Returns the name of the net connected to the pin.
    //! \param pinName    Name of the pin to be checked.
    //! \return           Net name connected to the pin, or an empty string if none.
    //!
    virtual QString   pinNetName( const QString pinName ) const override { return mPinRefMap.contains(pinName) ? mNetName : QString{}; }

    //!
    //! \brief unLinkPart Unlink part impelement from symbol implement
    //! \param partImp    Part implement
    //! \param undo       Undo operation
    //!
    virtual void      unLinkPart( SdGraphPartImp *partImp, SdUndo *undo ) override;


    // SdObject interface
  public:
    virtual QString   getType() const override  { return QStringLiteral(SD_TYPE_NET_PINS_LIST); }
    virtual SdClass   getClass() const override { return dctText; }
    virtual void      detach(SdUndo *undo) override;
    virtual void      cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next) override;
    virtual void      json(SdJsonWriter &js) const override;
    virtual void      json(const SdJsonReader &js) override;

    // SdGraph interface
  public:
    virtual void      saveState(SdUndo *undo) override;
    virtual void      transform(const QTransform &map, SdPvAngle angle) override;
    virtual void      setProp(SdPropSelected &prop) override;
    virtual void      getProp(SdPropSelected &prop) override;
    virtual void      selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void      selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void      setLayerUsage() override;
    virtual bool      isVisible() const override;
    virtual SdRect    getOverRect() const override { return mOverRect; }
    virtual void      draw(SdContext *dc) override;
    virtual int       behindCursor(SdPoint p) override;

  private:
    void disconnectAll();
    void connectAll();
  };

#endif // SDGRAPHNETPINSLIST_H
