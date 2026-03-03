/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
*/
#include "IgesEntityCompositeCurve.h"
#include "IgesReader.h"

IgesEntityCompositeCurve::IgesEntityCompositeCurve()
  {

  }


bool IgesEntityCompositeCurve::parse(IgesReader *reader)
  {
  if( !IgesEntity::parse(reader) ) return false;

  int count;
  if( !reader->paramInt( count ) ) return false;
  mList.clear();
  mList.reserve( count );
  for( int i = 0; i < count; i++ ) {
    int index;
    if( !reader->paramInt( index ) ) return false;
    mList.append( reader->entity(index) );
    }
  return true;
  }
