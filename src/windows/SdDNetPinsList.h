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


//!
//! \brief The SdDNetPinsList class
//! Dialog for editing net pin connections. Consists of a combo box for selecting/entering the net name,
//! a left list showing pins already assigned to the current net, and a right list showing all available
//! pins that are not yet connected to any net. Buttons allow moving pins between the lists.
//!
class SdDNetPinsList : public QDialog
  {
    Q_OBJECT

    SdProjectItem   *mSheet;            //!< Sheet object where net pin list resides
    QComboBox       *mNetName;          //!< Net name combo box - populated with all available nets in the project, allows manual entry
    QListWidget     *mPinListWidget;    //!< Left list widget displaying pins assigned to the current net
    QListWidget     *mSourcePinsWidget; //!< Right list widget displaying all available pins not yet connected to any net
    SdNetPinRefList  mPinList;          //!< Container for pins assigned to the current net
    SdNetPinRefList  mSourcePins;       //!< Container for all available pins that can be connected via text method
  public:
    SdDNetPinsList( SdProjectItem *obj, QWidget *parent );

    //!
    //! \brief setup      Initializes the dialog with the specified net name and pin list
    //! \param netName    The net name to display in the combo box
    //! \param pinRefList The list of pins currently assigned to this net
    //!
    void setup( const QString &netName, const SdNetPinRefList &pinRefList );

    //!
    //! \brief pinList Returns the current list of pins assigned to the net
    //! \return        SdNetPinRefList containing all pins in the left (assigned) list
    //!
    SdNetPinRefList pinList() const { return mPinList; }

    //!
    //! \brief netName Returns the currently selected or entered net name
    //! \return        QString containing the net name from the combo box
    //!
    QString         netName() const { return mNetName->currentText(); }

  public slots:
    //!
    //! \brief pinAppend Moves a pin from the right (available) list to the left (assigned) list
    //!                  Called when the "Add" button under the right table is clicked
    //!
    void            pinAppend();

    //!
    //! \brief pinRemove Moves a pin from the left (assigned) list to the right (available) list
    //!                  Called when the "Remove" button under the left table is clicked
    //!
    void            pinRemove();

  private:
    //!
    //! \brief fillSourcePins Populates the right (available) pins list
    //!
    //! Scans all schematic sheets, finds components with a "pins" field in their parameters,
    //! parses the parameter string to get the list of pins that can be connected via text method,
    //! and adds only those pins that are not yet connected to any net
    //!
    void            fillSourcePins();
  };

#endif // SDDNETPINSLIST_H
