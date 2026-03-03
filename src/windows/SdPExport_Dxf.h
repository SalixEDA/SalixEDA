/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Creation dxf file from current object
*/
#ifndef SDPEXPORT_DXF_H
#define SDPEXPORT_DXF_H

#include "SdPMasterList.h"
#include "objects/SdProjectItem.h"


class SdPExport_Dxf : public QWizardPage
  {
    Q_OBJECT

    SdProjectItem *mProjectItem; //Item from which will be made DXF file
  public:
    SdPExport_Dxf( SdProjectItem *item, int step, SdPMasterList *list, QWidget *parent = nullptr );

    // QWizardPage interface
  public:
    virtual bool validatePage() override;
    virtual int  nextId() const override { return -1; }

  public slots:
    //When press button "layers" we show layers dialog to on-off desired layers
    void onLayers();

  };

#endif // SDPEXPORT_DXF_H
