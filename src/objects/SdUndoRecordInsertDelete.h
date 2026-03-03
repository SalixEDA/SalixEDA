/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Undo record for inserted or deleted object
*/
#ifndef SDUNDORECORDINSERT_H
#define SDUNDORECORDINSERT_H

#include "SdUndoRecord.h"
#include "SdObject.h"
#include "SdContainer.h"

class SdUndoRecordInsertDelete : public SdUndoRecord
  {
    SdContainer *mContainer;
    SdObject    *mObject;
    bool         mInsert;
  public:
    SdUndoRecordInsertDelete( SdContainer *container, SdObject *object, bool insert );

    // SdUndoRecord interface
  public:
    //Undo elementary operation.
    virtual void undo() override;

    //Redo elementary operation.
    virtual void redo() override;
  };

#endif // SDUNDORECORDINSERT_H
