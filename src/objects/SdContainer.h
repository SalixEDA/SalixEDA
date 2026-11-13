/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdContainer - container object contains other objects with param table

  Param table is map of key-value pairs. Value is a string.
*/

#ifndef SDCONTAINER_H
#define SDCONTAINER_H

#include "SdObject.h"
#include "SdPoint.h"
#include "SdParamMap.h"

#include <QList>
#include <QMap>
#include <functional>

class SdContext;

using SdObjectPtrList = QList<SdObjectPtr>;

class SdContainer : public SdObject, public SdParamMap
  {
  protected:
    SdObjectPtrList  mChildList;  //!< List of child objects
  public:
    SdContainer();
    ~SdContainer() override;

    //Information
    virtual bool isContainer() const override { return true; }

    virtual void insertChild( SdObject *child, SdUndo *undo );
    virtual void undoInsertChild( SdObject *child );
    virtual void redoInsertChild( SdObject *child );

    virtual void deleteChild( SdObject *child, SdUndo *undo );
    virtual void undoDeleteChild( SdObject *child );
    virtual void redoDeleteChild( SdObject *child );


    //!
    //! \brief cloneFrom Overrided function. We copy all objects of source container
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief isChild Test if object obj is child of this container or not
    //! \param obj     Tested object
    //! \return        true if object is child of this container
    //!
    bool         isChild( const SdObject *obj ) const;

            void deleteAll( SdUndo *undo );

            void draw( SdContext *context );
/*[] (SdObject *obj) -> bool {

   }
   */
            void forEach( quint64 classMask, std::function<bool(SdObject*)> fun1 );
            void forEachConst( quint64 classMask, std::function<bool(SdObject*)> fun1 ) const;

            void sort( std::function<bool(SdObject *obj1, SdObject *obj2)> fun1 );

    //Return object and its state behind point
    SdObject    *behindPoint( quint64 classMask, SdPoint p, int *state );


    //!
    //! \brief setDirtyFlagForParamMap Mark the owner state as modified after parameter changes
    //!
    virtual void setDirtyFlagForParamMap() override { setProjectDirtyFlag(); }

    //Return param value from hierarchy. We start from local table
    // if param present in local table - return it
    // If no param in local table then we look at parent container
    // if no parent then return empty string
    QString      paramHierarchy( const QString key ) const;


  private:
    void         clearChildList();

    // SdObject interface
  public:
    //Return true if used any of objects in container but exclude itself objects in container
    virtual bool isUsed(SdObject *test) const override;

    //Upgrade project item on new one
    virtual bool upgradeProjectItem( SdUndo *undo, QWidget *parent ) override;

    //Upgrage only class objects with newer items
    bool         upgradeClassProjectItem(SdClass mask, SdUndo *undo , QWidget *parent);

    // SdObject interface
  public:
    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void json( const SdJsonReader &js ) override;
  };


#endif // SDCONTAINER_H
