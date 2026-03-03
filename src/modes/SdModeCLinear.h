/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Base mode for insert linear objects
*/
#ifndef SDMODECLINEAR_H
#define SDMODECLINEAR_H

#include "SdModeCommon.h"
#include "objects/SdPropLine.h"

class SdModeCLinear : public SdModeCommon
  {
  public:
    SdModeCLinear( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
    virtual int  getPropBarId() const override;
    virtual void propGetFromBar() override;
    virtual void propSetToBar() override;
  };

#endif // SDMODECLINEAR_H
