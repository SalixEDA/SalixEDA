/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Undo record for one string map pair
*/
#ifndef SDUNDORECORDSTRINGMAPITEM_H
#define SDUNDORECORDSTRINGMAPITEM_H

#include "SdUndoRecord.h"
#include "library/SdStringMap.h"

class SdUndoRecordStringMapItem : public SdUndoRecord
  {
    SdStringMap *mStringMap;
    QString      mKey;
    QString      mValue;
    bool         mPresence;   //True if key was present in map
  public:
    SdUndoRecordStringMapItem( SdStringMap *map, const QString key );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
    virtual void redo() override;
  };


#endif // SDUNDORECORDSTRINGMAPITEM_H
