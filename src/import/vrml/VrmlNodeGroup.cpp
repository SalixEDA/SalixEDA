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
#include "VrmlNodeGroup.h"
#include "SdScanerVrml.h"

VrmlNodeGroup::VrmlNodeGroup()
  {

  }

VrmlNodeGroup::VrmlNodeGroup(const VrmlNodeGroup *group) :
  VrmlNodeCompound( group )
  {
  mBoxCenter = group->mBoxCenter;
  mBoxSize   = group->mBoxSize;
  }



bool VrmlNodeGroup::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("children") )
    parseChildren( scaner );
  else if( fieldType == QStringLiteral("bboxCenter") )
    mBoxCenter.parse( scaner );
  else if( fieldType == QStringLiteral("bboxSize") )
    mBoxSize.parse( scaner );
  else return false;
  return true;
  }
