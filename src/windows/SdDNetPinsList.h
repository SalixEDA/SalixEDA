/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Dialog for edit net pins list
*/
#ifndef SDDNETPINSLIST_H
#define SDDNETPINSLIST_H

#include "objects/SdNetPinRef.h"

#include <QDialog>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>



class SdDNetPinsList : public QDialog
  {
    Q_OBJECT

    SdProjectItem   *mSheet;            //!< Sheet object where net pin list resides
    QComboBox       *mNetName;          //!< Net name field
    QListWidget     *mPinListWidget;    //!< Net pin list widget
    QListWidget     *mSourcePinsWidget; //!< Available pins wdiget for net
    SdNetPinRefList  mPinList;          //!< Net pin list
    SdNetPinRefList  mSourcePins;       //!< Available pins for net
  public:
    SdDNetPinsList( SdProjectItem *obj, QWidget *parent );

    void setup( const QString &netName, const SdNetPinRefList &pinRefList );

    SdNetPinRefList pinList() const { return mPinList; }

    QString         netName() const { return mNetName->currentText(); }

  public slots:
    void            pinAppend();

    void            pinRemove();

  private:
    void            fillSourcePins();
  };

#endif // SDDNETPINSLIST_H
