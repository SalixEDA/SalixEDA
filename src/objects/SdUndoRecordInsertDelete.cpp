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
#include "SdUndoRecordInsertDelete.h"

SdUndoRecordInsertDelete::SdUndoRecordInsertDelete(SdContainer *container, SdObject *object, bool insert) :
  SdUndoRecord(),
  mContainer(container),
  mObject(object),
  mInsert(insert)
  {

  }


void SdUndoRecordInsertDelete::undo()
  {
  if( mInsert )
    mContainer->undoInsertChild( mObject );
  else
    mContainer->undoDeleteChild( mObject );
  }




void SdUndoRecordInsertDelete::redo()
  {
  if( mInsert )
    mContainer->redoInsertChild( mObject );
  else
    mContainer->redoDeleteChild( mObject );
  }
