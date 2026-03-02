/*
Project "Electronic schematic and pcb CAD"

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Common mode for some graph modes
*/

#ifndef SDMODECOMMON_H
#define SDMODECOMMON_H

#include "SdMode.h"

class SdModeCommon : public SdMode
  {
  protected:
    SdPoint mPrev; //Предыдущая точка (при перемещении)
  public:
    SdModeCommon( SdWEditorGraph *editor, SdProjectItem *obj );

  };

#endif // SDMODECOMMON_H
