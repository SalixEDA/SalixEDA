/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Dialog for selection from list of string
*/
#include "SdDStringFromList.h"
#include "ui_SdDStringFromList.h"

#include <QListWidget>
#include <QTextBrowser>




SdDStringFromList::SdDStringFromList( const QStringList &lst, std::function<QString( QString key )> fun, QWidget *parent) :
  QDialog(parent),
  mGetDescription(fun),
  ui(new Ui::SdDStringFromList)
  {
  ui->setupUi(this);

  ui->mList->addItems( lst );
  //When current row changed we change description for this row
  connect( ui->mList, &QListWidget::currentRowChanged, this, [this] (int row ) {
    Q_UNUSED(row)
    QString str = ui->mList->currentItem()->text();
    ui->mDescription->setText( mGetDescription(str) );
    } );
  ui->mList->setCurrentRow(0);

  // Assign unique names for the interactive help system
  ui->mList->setObjectName("SdDStringFromList.mList");
  ui->mDescription->setObjectName("SdDStringFromList.mDescription");
  }




SdDStringFromList::~SdDStringFromList()
  {
  delete ui;
  }




//Result of selection
QString SdDStringFromList::result() const
  {
  return ui->mList->currentItem()->text();
  }
