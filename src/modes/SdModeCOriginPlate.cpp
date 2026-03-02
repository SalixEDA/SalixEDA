/*
Project "Electronic schematic and pcb CAD"

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for plate origin
*/
#include "SdModeCOriginPlate.h"

SdModeCOriginPlate::SdModeCOriginPlate(SdWEditorGraph *editor, SdProjectItem *obj, int osize) :
  SdModeCOrigin( editor, obj, osize )
  {
  mModeIndex = MD_PLATE_ORIGIN;
  }
