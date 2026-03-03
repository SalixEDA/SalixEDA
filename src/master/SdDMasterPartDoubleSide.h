/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Part master with rectangle body and two pins columns by left and right sides
*/
#ifndef SDDMASTERPARTDOUBLESIDE_H
#define SDDMASTERPARTDOUBLESIDE_H

#include "SdDMasterPart.h"
#include "windows/SdIllustrator.h"

namespace Ui {
  class SdDMasterPartDoubleSide;
}

class SdDMasterPartDoubleSide : public SdDMasterPart
  {
    Q_OBJECT

  public:
    explicit SdDMasterPartDoubleSide( SdProjectItem *item, QWidget *parent = nullptr);
    ~SdDMasterPartDoubleSide() override;

  public slots:
    //Update preview on any params changed
    void onEditChanged( const QString txt );

  private:
    Ui::SdDMasterPartDoubleSide *ui;

    //Draw part preview
    void drawPart( SdIllustrator &il );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };


#endif // SDDMASTERPARTDOUBLESIDE_H
