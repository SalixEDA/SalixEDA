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

  The dialog consists of a combo box for the net name. We populate
  it with a list of all available nets in the project. We also allow
  entering a net name manually.

  In the central part there are two lists: a list of pins for the
  given net (left) and a list of all available and not yet connected
  pins (right). We populate the right list when constructing the dialog.
  To do this, we scan all schematic sheets. In each schematic sheet, we
  look for component occurrences, and for each component, we check for
  the presence of a "pins" field in its parameters. If the field exists,
  then this component is a candidate for adding to the table. Next, we
  parse the parameter string and obtain a list of pins that can be
  connected via text method. After that, we check whether these pins
  are already connected. If they are not connected, they are added to
  the available pins table.

  Below the tables, there are two buttons: under the left table,
  a "Remove" button, which moves a pin from the left table to the right;
  and under the right table, an "Add" button, which moves a pin from
  the right table to the left. Thus, using these buttons, you can move
  pins back and forth between the two lists.

  When the OK button of the dialog is pressed, the changes take effect
  with the possibility of undoing. When the Cancel button is pressed,
  all changes are ignored.
*/
#include "SdDNetPinsList.h"
#include "SdDHelp.h"
#include "objects/SdProject.h"
#include "objects/SdProjectItem.h"
#include "objects/SdGraphSymImp.h"
#include "objects/SdGraphPartImp.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>


SdDNetPinsList::SdDNetPinsList(SdProjectItem *obj, QWidget *parent)
  : QDialog(parent)
  , mSheet(obj)
  {
  //Build interface
  QVBoxLayout *vbox = new QVBoxLayout();
  QHBoxLayout *hbox = new QHBoxLayout();

  hbox->addWidget( new QLabel(tr("Net name:")) );
  hbox->addWidget( mNetName = new QComboBox() );
  mNetName->setEditable(true);
  mNetName->setToolTip( tr("Net name for pins") );

  vbox->addLayout( hbox );


  QGridLayout *glay = new QGridLayout();

  glay->addWidget( new QLabel(tr("Net pins:")), 0, 0 );
  glay->addWidget( new QLabel(tr("Available pins:")), 0, 1 );

  glay->addWidget( mPinListWidget = new QListWidget(), 1, 0 );
  glay->addWidget( mSourcePinsWidget = new QListWidget(), 1, 1 );

  QPushButton *butAppend;
  QPushButton *butRemove;

  glay->addWidget( butRemove = new QPushButton( tr("Remove >>") ), 2, 0 );
  glay->addWidget( butAppend = new QPushButton( tr("<< Append") ), 2, 1 );

  connect( butRemove, &QPushButton::clicked, this, &SdDNetPinsList::pinRemove );
  connect( butAppend, &QPushButton::clicked, this, &SdDNetPinsList::pinAppend );

  vbox->addLayout( glay );

  QDialogButtonBox *box = new QDialogButtonBox( QDialogButtonBox::Ok|QDialogButtonBox::Cancel|QDialogButtonBox::Help, this);

  connect( box, &QDialogButtonBox::accepted, this, &SdDNetPinsList::accept );
  connect( box, &QDialogButtonBox::rejected, this, &SdDNetPinsList::reject );
  //Help system
  connect( box->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDNetPinsList.htm"), this );
    } );

  vbox->addWidget( box );

  setLayout( vbox );
  mNetName->setFocus();

  //Fill available pins list
  fillSourcePins();

  std::sort( mSourcePins.begin(), mSourcePins.end() );

  //Fill visual source pin table
  for( auto const &pin : std::as_const(mSourcePins) ) {
    mSourcePinsWidget->addItem( QString("%1 %2 %3").arg( pin.mPinFunction, pin.mPartImp->ident(), pin.mPinNumber ) );
    }

  // Assign unique names for the interactive help system
  mNetName->setObjectName( "SdDNetPinsList.mNetName" );
  mPinListWidget->setObjectName( "SdDNetPinsList.mPinListWidget" );
  mSourcePinsWidget->setObjectName( "SdDNetPinsList.mSourcePinsWidget" );
  }




//!
//! \brief setup      Initializes the dialog with the specified net name and pin list
//! \param netName    The net name to display in the combo box
//! \param pinRefList The list of pins currently assigned to this net
//!
void SdDNetPinsList::setup(const QString &netName, const SdNetPinRefList &pinRefList)
  {
  mNetName->setCurrentText( netName );
  mPinList = pinRefList;

  //Fill pin table
  for( const auto &pin : std::as_const(mPinList) )
    mPinListWidget->addItem( QString("%1 %2 %3").arg( pin.mPinFunction, pin.mPartImp->ident(), pin.mPinNumber ) );
  }



//!
//! \brief pinAppend Moves a pin from the right (available) list to the left (assigned) list
//!                  Called when the "Add" button under the right table is clicked
//!
void SdDNetPinsList::pinAppend()
  {
  int index = mSourcePinsWidget->currentRow();
  if( index >= 0 ) {
    QString str = mSourcePinsWidget->item(index)->text();
    delete mSourcePinsWidget->item(index);
    auto pin = mSourcePins.takeAt( index );

    mPinListWidget->addItem( str );
    mPinList.append( pin );
    }
  }



//!
//! \brief pinRemove Moves a pin from the left (assigned) list to the right (available) list
//!                  Called when the "Remove" button under the left table is clicked
//!
void SdDNetPinsList::pinRemove()
  {
  int index = mPinListWidget->currentRow();
  if( index >= 0 ) {
    QString str = mPinListWidget->item(index)->text();
    delete mPinListWidget->item(index);
    auto pin = mPinList.takeAt( index );

    mSourcePinsWidget->addItem( str );
    mSourcePins.append( pin );
    }
  }




//!
//! \brief fillSourcePins Populates the right (available) pins list
//!
//! Scans all schematic sheets, finds components with a "pins" field in their parameters,
//! parses the parameter string to get the list of pins that can be connected via text method,
//! and adds only those pins that are not yet connected to any net
//!
void SdDNetPinsList::fillSourcePins()
  {
  if( mSheet != nullptr ) {
    //Get project
    SdProject *project = mSheet->getProject();
    if( project != nullptr ) {
      //Scan all sheets, in sheet we scan all components and extract pins from them
      //Pins must be format: "pins| VCC=14; GND=7"
      project->forEachConst( dctSheet, [this] (SdObject *obj) -> bool {
        SdPtrConst<SdProjectItem> itemPtr(obj);
        if( itemPtr.isValid() ) {
          itemPtr->forEachConst( dctSymImp, [this] (SdObject *obj) -> bool {
            SdPtrConst<SdGraphSymImp> symImpPtr(obj);
            if( symImpPtr.isValid() ) {
              //Get Pins param from component
              if( symImpPtr->paramContains( stdParamPins ) ) {
                QString pinsString = symImpPtr->paramGet( stdParamPins ).simplified();
                //Get part imp
                SdGraphPartImp *partImp = symImpPtr->getPartImp();

                if( !pinsString.isEmpty() && partImp != nullptr ) {
                  //Parse pins list
                  QStringList pins = pinsString.split( QChar(';') );
                  for( auto const &singleAssoc : std::as_const(pins) ) {
                    QStringList pair = singleAssoc.split( QChar('=') );
                    if( pair.count() == 2 ) {
                      if( partImp->isPinNotLinked( pair.at(1).simplified() ) ) {
                        SdNetPinRef pinRef;
                        pinRef.mPartImp     = partImp;
                        pinRef.mPinNumber   = pair.at(1).simplified();
                        pinRef.mPinFunction = pair.at(0).simplified();
                        bool found = false;
                        for( auto const &pin : std::as_const(mSourcePins) ) {
                          found = pin.isEqual( pinRef );
                          if( found ) break;
                          }

                        if( !found )
                          mSourcePins.append( pinRef );
                        }
                      }
                    }
                  }
                }
              }
            return true;
            });
          }
        return true;
        });

      //Fill net list
      mNetName->addItems( project->netList() );
      }
    }
  }
