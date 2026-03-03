/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Beginning undo record. It define undo title and foregraund edit object.
*/

#include "SdUndoRecordBegin.h"
#include "SdPulsar.h"

void SdUndoRecordBegin::undo()
  {
  //Bring to top edit object
  if( mIs3d )
    SdPulsar::sdPulsar->emitActivateItem3d( mEditItem );
  else
    SdPulsar::sdPulsar->emitActivateItem( mEditItem );
  }
