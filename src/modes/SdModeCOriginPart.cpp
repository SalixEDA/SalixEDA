/*
Project "Electronic schematic and pcb CAD"

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for part pin origin
*/
#include "SdModeCOriginPart.h"

SdModeCOriginPart::SdModeCOriginPart( SdWEditorGraph *editor, SdProjectItem *obj, int osize ) :
  SdModeCOrigin( editor, obj, osize )
  {
  mModeIndex = MD_PART_ORIGIN;
  }
