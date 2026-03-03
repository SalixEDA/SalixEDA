/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Undo record for implement symbol pin properties and point.
*/
#ifndef SDUNDORECORDSYMIMPPIN_H
#define SDUNDORECORDSYMIMPPIN_H

#include "SdUndoRecord.h"
#include "SdSymImpPin.h"

class SdGraphSymImp;
class SdGraphPartImp;

class SdUndoRecordSymImpPin : public SdUndoRecord
  {
    SdGraphSymImp  *mSymImp;
    QString         mPinName;   //Pin name in symbol
    SdSymImpPin     mPinState;  //Symbol implement pin state
  public:
    SdUndoRecordSymImpPin( SdGraphSymImp *imp, QString pinName );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDSYMIMPPIN_H
