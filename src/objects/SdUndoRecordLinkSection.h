/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Undo link beatween symbol implementation and part implementation
*/
#ifndef SDUNDORECORDLINKSECTION_H
#define SDUNDORECORDLINKSECTION_H

#include "SdUndoRecord.h"

class SdGraphSymImp;
class SdGraphPartImp;

class SdUndoRecordLinkSection : public SdUndoRecord
  {
    int             mSection;
    bool            mLink;
    SdGraphSymImp  *mSym;
    SdGraphPartImp *mPart;
  public:
    SdUndoRecordLinkSection(int section, SdGraphSymImp *sym, SdGraphPartImp *part , bool link);

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDLINKSECTION_H
