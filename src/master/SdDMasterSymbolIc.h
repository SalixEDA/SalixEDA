/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Symbol creation master for integrated circuits
*/
#ifndef SDDMASTERSYMBOLIC_H
#define SDDMASTERSYMBOLIC_H

#include "SdDMasterSymbol.h"
#include "windows/SdIllustrator.h"

namespace Ui {
    class SdDMasterSymbolIc;
  }

class SdDMasterSymbolIc : public SdDMasterSymbol
  {
    Q_OBJECT

  public:
    explicit SdDMasterSymbolIc( SdProjectItem *item, QWidget *parent = nullptr );
    ~SdDMasterSymbolIc() override;


  public slots:
    //Update preview on any params changed
    void onEditChanged( const QString txt );

    //Update preview on any pin changes
    void onPinChanged( int row, int column );

    //Insert new pin to left side
    void leftInsertPin();

    //Insert new gap to left side
    void leftInsertGap();

    //Insert new delimiter to left side
    void leftInsertDelimiter();

    //Delete pin, gap or delimiter from left side
    void leftDelete();

    //Insert new pin to right side
    void rightInsertPin();

    //Insert new gap to right side
    void rightInsertGap();

    //Insert new delimiter to right side
    void rightInsertDelimiter();

    //Delete pin, gap or delimiter from right side
    void rightDelete();


  private:
    Ui::SdDMasterSymbolIc *ui;

    //Draw symbol preview
    void drawSymbol( SdIllustrator &il );


    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDMASTERSYMBOLIC_H
