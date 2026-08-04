/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Dialog for get objects from project. Dialog listing existing objects of desired type and allow selected one of it
*/
#include "SdDGetProjectObject.h"
#include "ui_SdDGetProjectObject.h"
#include "objects/SdProjectItem.h"
#include "objects/SdPItemComponent.h"
#include "objects/SdPItemSymbol.h"
#include "objects/SdSection.h"
#include "objects/SdPartVariant.h"

#include <QHBoxLayout>
#include <QMessageBox>

SdDGetProjectObject::SdDGetProjectObject(SdContainer *project, quint64 mask, const QString title, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDGetProjectObject)
  {
  ui->setupUi(this);
  setWindowTitle( title );

  //Create item view
  mView = new SdWEditorGraphView( ui->mViewBox );
  QHBoxLayout *lay = new QHBoxLayout();
  ui->mViewBox->setLayout( lay );
  lay->addWidget( mView );

  //Fill list
  ui->mList->setSortingEnabled(false);
  project->forEach( mask, [this] (SdObject *obj) ->bool {
    SdProjectItem *item = dynamic_cast<SdProjectItem*>(obj);
    if( item ) {
      //Append item into visual list as text string and index of object in internal list
//      ui->mList->addItem( new QListWidgetItem( item->getExtendTitle(), nullptr, mList.count() )  );
      ui->mList->addItem( item->getExtendTitle() );
      //Append item into internal list
      mList.append( item );
      }
    return true;
    });

  //Reaction on selection is set object for preview
  connect( ui->mList, &QListWidget::currentRowChanged, this, [this] ( int row ) {
    if( row >= 0 && row < mList.count() ) {
      mView->setItem( mList.at(row), false );
      }
    });

  // Assign unique names for the interactive help system
  ui->mList->setObjectName("SdDGetProjectObject.mList");
  ui->mViewBox->setObjectName("SdDGetProjectObject.mViewBox");
  mView->setObjectName("SdDGetProjectObject.mView");
  }




SdDGetProjectObject::~SdDGetProjectObject()
  {
  delete ui;
  }




SdProjectItem *SdDGetProjectObject::selectedItem() const
  {
  int row = ui->mList->currentRow();
  if( row >= 0 && row < mList.count() )
    return mList.at(row);
  return nullptr;
  }




SdProjectItem *SdDGetProjectObject::getItem(SdContainer *project, quint64 mask, const QString title, QWidget *parent)
  {
  SdDGetProjectObject dlg( project, mask, title, parent );
  if( dlg.exec() )
    return dlg.selectedItem();
  return nullptr;
  }

