/*
Project "Electronic schematic and pcb CAD"

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Symbol implement hightlight mode.
*/
#ifndef SDMODECSYMHIGHLIGHT_H
#define SDMODECSYMHIGHLIGHT_H

#include "SdModeCommon.h"

class SdModeCSymHighlight : public SdModeCommon
  {
    SdGraphSymImp *mSymImp; //!< Highlighted symbol implement or nullptr if none
  public:
    SdModeCSymHighlight( SdWEditorGraph *editor, SdProjectItem *obj );
  };

#endif // SDMODECSYMHIGHLIGHT_H
