/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Converter for simple offset. Used for display copy of selected objects in appropriate place
*/
#include "SdConverterOffset.h"


SdConverterOffset::SdConverterOffset(SdPoint offset) :
  mOffset(offset)
  {

  }


QTransform SdConverterOffset::getMatrix()
  {
  return QTransform::fromTranslate( mOffset.x(), mOffset.y() );
  }

