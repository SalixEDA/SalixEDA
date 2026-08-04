/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Dialog for handle enter position of cursor
*/
#include "SdDEnterPosition.h"
#include "ui_SdDEnterPosition.h"
#include "SdDHelp.h"

static bool    prevRef = false;
static QString prevX("0"),prevY("0"),prevZ("0");

SdDEnterPosition::SdDEnterPosition(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDEnterPosition)
  {
  ui->setupUi(this);

  //Restore previous control state
  ui->mRef->setChecked(prevRef);
  ui->mX->setText( prevX );
  ui->mY->setText( prevY );
  ui->mZ->setText( prevZ );

  //Help system
  connect( ui->buttonBox, &QDialogButtonBox::helpRequested, this, [this] () { SdDHelp::help( "SdDEnterPosition.htm", this ); });

  // Assign unique names for the interactive help system
  ui->mRef->setObjectName("SdDEnterPosition.mRef");
  ui->mX->setObjectName("SdDEnterPosition.mX");
  ui->mY->setObjectName("SdDEnterPosition.mY");
  ui->mZ->setObjectName("SdDEnterPosition.mZ");
  }




SdDEnterPosition::~SdDEnterPosition()
  {
  delete ui;
  }




bool SdDEnterPosition::getRef() const
  {
  return ui->mRef->isChecked();
  }



QString SdDEnterPosition::getX() const
  {
  return ui->mX->text();
  }



QString SdDEnterPosition::getY() const
  {
  return ui->mY->text();
  }



QString SdDEnterPosition::getZ() const
  {
  return ui->mZ->text();
  }



void SdDEnterPosition::changeEvent(QEvent *e)
  {
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
    }
  }


void SdDEnterPosition::accept()
  {
  prevRef = ui->mRef->isChecked();
  prevX = ui->mX->text();
  prevY = ui->mY->text();
  prevZ = ui->mZ->text();
  QDialog::accept();
  }
