/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Symbol creation master for integrated circuits with pin numbers
*/
#ifndef SDDMASTERSYMBOLICWITHNUM_H
#define SDDMASTERSYMBOLICWITHNUM_H

#include "SdDMasterSymbol.h"
#include "windows/SdIllustrator.h"

#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>


class SdDMasterSymbolIcWithNum : public SdDMasterSymbol
  {
    Q_OBJECT

    QTableWidget *mLeftTable;
    QTableWidget *mRightTable;
    QTableWidget *mCentralTable;
    QLabel       *mPreview;
    QLineEdit    *mTitle;

  public:
    explicit SdDMasterSymbolIcWithNum( SdProjectItem *item, QWidget *parent = nullptr );


  public slots:
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

    void moveLeft();

    void moveRight();

    void fromClipboard();

  private:

    //Draw symbol preview
    void drawSymbol( SdIllustrator &il );

    void leftInsert( const QString &pinName, const QString &pinNumber );

    void rightInsert( const QString &pinName, const QString &pinNumber );

    void addAttr( int x, int y, int attr );

    bool addSymbol( const QString &pinName, int x, int y );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };


#endif // SDDMASTERSYMBOLICWITHNUM_H
