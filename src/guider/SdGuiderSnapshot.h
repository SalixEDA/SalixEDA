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
#ifndef SDGUIDERSNAPSHOT_H
#define SDGUIDERSNAPSHOT_H

#include "objects/SdJsonIO.h"

struct SdGuiderSnapshot
  {
    QString mProjectName; //!< Project name for object
    QString mObjectName;  //!< Object name itself
    quint64 mObjectClass; //!< Object class, because objects of different classes may have same names
    //2d graphics editors
    SdPoint mGrid;        //!< Editor grid
    SdPoint mOrigin;      //!< Editor view origin
    double  mScale;       //!< Editor view scale
    bool    mIs3d;        //!< 3d editor flag

    //!
    //! \brief json Function to write object into json writer
    //! \param js   Json writer
    //!
    void json( SdJsonWriter &js ) const;

    //!
    //! \brief json Function to read object from json reader
    //! \param js   Json reader
    //!
    void json( const SdJsonReader &js );
  };


using SdGuiderSnapshotList = QList<SdGuiderSnapshot>;

#endif // SDGUIDERSNAPSHOT_H
