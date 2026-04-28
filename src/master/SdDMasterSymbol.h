/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Base class for symbol masters.

  Contains operations for append graphics and pins to symbol when creation.
*/
#ifndef SDDMASTERSYMBOL_H
#define SDDMASTERSYMBOL_H

#include "objects/SdProjectItem.h"

#include <QDialog>


class SdDMasterSymbol : public QDialog
  {
    Q_OBJECT

  protected:
    SdProjectItem *mItem;
    SdPropLine     mLineProp;       //All graph objects append by default to "component" layer with 0-width
    SdPropSymPin   mPinProp;        //All pins
    SdPropText     mIdentProp;
    SdPropText     mValueProp;
    SdPropText     mPinNumberProp;
    SdPropText     mPinNameProp;


    //Add common graphics to part
    void addLine( int x1, int y1, int x2, int y2 );
    void addRect( int x1, int y1, int x2, int y2 );
    void addCircle( int cx, int cy, int r );
    void addText( int x, int y, const SdPropText &prp, const QString &text );

    //Identifier append to "id" layer
    void setId(SdPoint p, int size = 350 );

    //Value append to "value" layer
    void setValue( SdPoint p, int size = 350 );

    //Pin append to "pin" layer
    void addPin( SdPoint org, int type, SdPoint pinNameOrg, const QString pinName, SdPoint pinNumberOrg, const QString &pinNumber = QString{} );
  public:
    SdDMasterSymbol( SdProjectItem *item, QWidget *parent );

  };

#endif // SDDMASTERSYMBOL_H
