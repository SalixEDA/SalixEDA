/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Graphical object Part pin.
*/
#ifndef SDGRAPHPARTPIN_H
#define SDGRAPHPARTPIN_H

#include "SdGraph.h"
#include "SdPoint.h"
#include "SdPropText.h"
#include "SdPropPartPin.h"
#include "SdPvStratum.h"

#define SD_TYPE_PART_PIN "PartPin"

class SdPItemPlate;

struct SdPartPad
  {
    SdPoint     mOffset;  //!< Offset from pin anchor
    SdPvStratum mSide;    //!< Side of pin location [Сторона расположения вывода]
    SdPvString  mPinType; //!< Pin type for pad association [Тип вывода для привязки контактной площадки]

    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void json( const SdJsonReader &js);

    //!
    //! \brief swap Swap all part pin properties with another instance
    //! \param other Other SdPropPartPin object to swap with
    //!
    void swap( SdPartPad &other );

    void transform(const QTransform &map, SdPvAngle angle);
  };

using SdPartPadList = QList<SdPartPad>;

class SdGraphPartPin : public SdGraph
  {
    SdPoint       mOrigin;     //!< Pin origin anchor
    SdPropPartPin mPinProp;    //!< Pin properties
    SdPoint       mNamePos;    //!< Pin name position
    SdRect        mNameRect;   //!< Pin name over rect
    SdPropText    mNameProp;   //!< Pin name properties
    SdPoint       mNumberPos;  //!< Pin number position
    SdRect        mNumberRect; //!< Pin number over rect
    SdPropText    mNumberProp; //!< Pin number properties
    QString       mNumber;     //!< Pin number
    SdPartPadList mPadList;    //!< List of addon geometry pad forms

    //Different pin part selection
    bool          mPinSelect;  //!< Pin selected
    bool          mNumSelect;  //!< Pin number selected
    bool          mNamSelect;  //!< Pin name selected
    QList<int>    mPadSelect;  //!< Addon pads selection
  public:
    SdGraphPartPin();
    SdGraphPartPin( SdPoint org, SdPoint numberPos, SdPoint namePos, const QString number );
    SdGraphPartPin( SdPoint org, const SdPropPartPin &pinProp, SdPoint numberPos, const SdPropText &numberProp, SdPoint namePos, const SdPropText &nameProp, const QString number );

    //Return pin number
    QString     getPinNumber() const { return mNumber; }
    //Return pin type
    QString     getPinType() const { return mPinProp.mPinType.string(); }
    //Return pin origin
    SdPoint     getPinOrigin() const { return mOrigin; }
    //Return pin stratum
    SdPvStratum getPinStratum( bool otherSide ) const;

    void        drawImp(SdContext *dc, const QString pinName, bool com );

    const SdPartPadList &getPartPadList() const { return mPadList; }

    // SdObject interface
  public:
    virtual QString   getType() const override { return QString( SD_TYPE_PART_PIN ); }
    virtual quint64   getClass() const override { return dctPartPin; }
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void      cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void      json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void      json( const SdJsonReader &js ) override;

    // SdGraph interface
  public:
    virtual void      saveState(SdUndo *undo) override;
    virtual void      transform(const QTransform &map, SdPvAngle angle) override;
    virtual void      setProp(SdPropSelected &prop) override;
    virtual void      getProp(SdPropSelected &prop) override;
    virtual bool      layerReplace( const QStringList &pairs ) override;
    virtual void      setText(int index, QString sour, SdPropText &prop, QWidget *parent) override;
    virtual void      selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void      selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void      select(SdSelector *selector) override;
    virtual bool      isVisible() const override;
    virtual SdRect    getOverRect() const override;
    virtual void      draw(SdContext *dc) override;

    //!
    //! \brief draw3d Draws object in 3d space
    //! \param f      3d draw functions with predefined 3d context
    //!
    virtual void      draw3d( QOpenGLFunctions_2_0 *f ) const override;

    virtual int       behindCursor(SdPoint p) override;
    virtual int       behindText( SdPoint p, SdPoint &org, QString &dest, SdPropText &prop ) override;
    //Find snap point on object
    virtual void      snapPoint(SdSnapInfo *snap) override;

    friend class SdPasCadImport;
  };

#endif // SDGRAPHPARTPIN_H
