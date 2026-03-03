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
#include "SdUndoRecordLinkSection.h"
#include "SdGraphSymImp.h"
#include "SdGraphPartImp.h"

SdUndoRecordLinkSection::SdUndoRecordLinkSection(int section, SdGraphSymImp *sym, SdGraphPartImp *part, bool link ) :
  SdUndoRecord(),
  mSection(section),
  mLink(link),
  mSym(sym),
  mPart(part)
  {

  }


void SdUndoRecordLinkSection::undo()
  {
  if( mPart )
    mPart->setLinkSection( mSection, mLink ? mSym : nullptr );
  if( mSym )
    mSym->setLinkSection( mSection, mLink ? mPart : nullptr );
  mLink = !mLink;
  }
