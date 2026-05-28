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

#ifndef SDGRAPHOBJECT_H
#define SDGRAPHOBJECT_H

#include "SdObject.h"
#include "SdPoint.h"
#include "SdRect.h"
#include "SdPropSelected.h"
#include "3dr/Sd3drFace.h"

#include <QOpenGLFunctions_2_0>

class SdSelector;
class SdContext;
class SdSnapInfo;
class QWidget;
class SdProjectItem;

class SdGraph : public SdObject
  {
  protected:
    //Saved in file
    int         mGroupId;  //!< Object group id

    //Not saved in file
    SdSelector *mSelector; //!< Current visual editor selector
  public:
    SdGraph();

    //Select object
    SdSelector*    getSelector() { return mSelector; }
    bool           isSelected() const { return mSelector != nullptr; }

    //Visual object grouping
    //!
    //! \brief groupId Returns current group id
    //! \return        Current group id, 0 if no group assigned
    //!
    int            groupId() const { return mGroupId; }

    //!
    //! \brief groupIdSet Sets new group id for object, 0 for remove group id
    //! \param newGroupId New group id or 0 for remove group id
    //! \param undo       Undo object to save previous group id
    //!
    void           groupIdSet( int newGroupId, SdUndo *undo );

    //!
    //! \brief groupIdNew Returns new group id
    //! \return           New unical group id
    //!
    static int     groupIdNew();

    //Save object state
    virtual void   saveState( SdUndo *undo );
    virtual void   moveComplete( SdPoint grid, SdUndo *undo );

    //Изменение перемещением, вращением, зеркальностью и др.
    virtual void   transform( const QTransform &map, SdPvAngle angle = SdPvAngle(0) );

    //Изменение свойствами
    virtual void   setProp( SdPropSelected &prop );
    virtual void   getProp( SdPropSelected &prop );
    virtual bool   layerReplace( const QStringList &pairs ) { Q_UNUSED(pairs) return false; }

    //Изменение обновлением текста
    virtual void   setText(int index, QString sour, SdPropText &prop, QWidget *parent );

    //Варианты выделения
    virtual void   selectByPoint( const SdPoint p, SdSelector *selector );
    virtual void   selectByRect( const SdRect &r, SdSelector *selector );
    virtual void   select( SdSelector *selector );
    virtual void   prepareMove( SdUndo *undo );
    //Layer maintance
    virtual void   setLayerUsage();
    //Check if object is visible
    virtual bool   isVisible() const { return false; }
    //Get over rect for visible
    virtual SdRect getOverRect() const = 0;

    //!
    //! \brief draw Draws object in 2d space [Рисование объекта на экране]
    //! \param dc   Display context where drawing doing
    //!
    virtual void   draw( SdContext *dc );

    //!
    //! \brief drawExceptText Draws object in 2d space with exception textual component with textId
    //! \param dc             Display context where drawing doing
    //! \param textId         Text id which must not be drawn
    //!
    virtual void   drawExceptText( SdContext *dc, int textId );

    //!
    //! \brief draw3d Draws object in 3d space
    //! \param f      3d draw functions with predefined 3d context
    //!
    virtual void   draw3d( QOpenGLFunctions_2_0 *f ) const;

    //!
    //! \brief accumHoles Accum holes description into faceList
    //! \param model      Model which accumulate coord vertexes
    //! \param faceList   Face list for holding holes (single face for single hole)
    //! \param stratum    Stratum for layers
    //! \param map        Map for holes conversion
    //!
    virtual void   accumHoles( Sd3drModel &model, Sd3drFaceList &faceList, SdPvStratum stratum, const QMatrix4x4 &map ) const;

    //Get object state behind cursor
    //!
    //! \brief behindCursor Return object status behind cursor, one of constant ELEM_xxx
    //! \param p            Point of cursor position
    //! \return             One of constant ELEM_xxx (outside any element, on selected element or on unselected element)
    //!
    virtual int    behindCursor( SdPoint p );

    //!
    //! \brief behindText Return text id behind cursor of this element
    //! \param p          Point of cursor position
    //! \param org        Origin point of textual component
    //! \param dest       Text of textual component
    //! \param prop       Properties of textual component
    //! \return           0 - if none textual components behind cursor or >0 (i.e. 1, 2 and so on) if any textual component
    //!
    //! Some elements may be have more than one textual component, for example pin. It has pin name and pin number
    //! So, for enable edit such components separately we return with this function Id for particular component
    virtual int    behindText( SdPoint p, SdPoint &org, QString &dest, SdPropText &prop );

    virtual bool   getInfo( SdPoint p, QString &info, bool extInfo );
    //Find snap point on object
    virtual void   snapPoint( SdSnapInfo *snap );


    //!
    //! \brief cloneFrom Clone contents object except mParent field. This must be overrided in all subclasses to
    //!                  generate right copy of object
    //!                  Cloned object has no parent
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void   cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief isClone Test if object src is clone of current object or viseversa
    //! \param src     Source object
    //! \return        true when object src is clone of current object or viseversa
    //!
    virtual bool   isClone( const SdObject *src ) const { Q_UNUSED(src) return false; }



    //Write and read object

    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    virtual void   json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    virtual void   json( const SdJsonReader &js ) override;

    static QMap<int,int> mGroupMap;

    friend class SdSelector;
  };

typedef SdGraph *SdGraphPtr;

#endif // SDGRAPHOBJECT_H
