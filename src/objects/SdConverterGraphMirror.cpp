/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  The converter performs mirroring across the segment. Used for display selected objects in appropriate place
*/
#include "SdConverterGraphMirror.h"


SdConverterGraphMirror::SdConverterGraphMirror(SdPoint from, SdPoint to)
  : mFrom(from)
  , mTo(to)
  {

  }

QTransform SdConverterGraphMirror::getMatrix()
  {
  return mFrom.transformMirror( mTo );
  }
