/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  PCB board builder. In dialog form we create board rectangle
  and fix holes.
*/

#include "SdDBoardBuilder.h"
#include "SdDHelp.h"
#include "objects/SdSelector.h"
#include "objects/SdGraphLinear.h"
#include "objects/SdGraphLinearRect.h"
#include "objects/SdGraphLinearCircle.h"
#include "objects/SdEnvir.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>

SdDBoardBuilder::SdDBoardBuilder(SdProjectItem *item, QWidget *parent) :
  QDialog(parent)
  {
  mPlate = dynamic_cast<SdPItemPlate*>(item);

  //At top - board configuration selector
  QVBoxLayout *vbox = new QVBoxLayout(this);
  QGridLayout *grid = new QGridLayout();
  QLabel *label;
  QRadioButton *but;
  mBoardVariant = new QButtonGroup(this);

  grid->addWidget( label = new QLabel(), 0, 0 );
  label->setPixmap( QPixmap(QString(":/pic/boardX0.png")) );
  grid->addWidget( but = new QRadioButton(tr("Without holes")), 1, 0 );
  mBoardVariant->addButton( but, 0 );

  grid->addWidget( label = new QLabel(), 0, 1 );
  label->setPixmap( QPixmap(QString(":/pic/boardX2.png")) );
  grid->addWidget( but = new QRadioButton(tr("With 2 holes")), 1, 1 );
  mBoardVariant->addButton( but, 2 );

  grid->addWidget( label = new QLabel(), 0, 2 );
  label->setPixmap( QPixmap(QString(":/pic/boardX4.png")) );
  grid->addWidget( but = new QRadioButton(tr("With 4 holes")), 1, 2 );
  mBoardVariant->addButton( but, 4 );

  grid->addWidget( label = new QLabel(), 0, 3 );
  label->setPixmap( QPixmap(QString(":/pic/boardX6.png")) );
  grid->addWidget( but = new QRadioButton(tr("With 6 holes")), 1, 3 );
  mBoardVariant->addButton( but, 6 );

  mBoardVariant->button(4)->setChecked(true);

  vbox->addLayout( grid );


  QFormLayout *form = new QFormLayout();

  form->addRow( tr("Board size X (horizontal):"), mSizeX = new QLineEdit("70") );
  form->addRow( tr("Board size Y (vertical):"), mSizeY = new QLineEdit("40") );
  form->addRow( tr("All Holes diametr:"), mHoleDiam = new QLineEdit("3.5") );
  form->addRow( tr("Hole center to edge board distance:"), mHoleDist = new QLineEdit("5") );

  vbox->addLayout( form );

  QDialogButtonBox *box = new QDialogButtonBox( QDialogButtonBox::Ok|QDialogButtonBox::Cancel|QDialogButtonBox::Help, this);

  connect( box, &QDialogButtonBox::accepted, this, &SdDBoardBuilder::accept );
  connect( box, &QDialogButtonBox::rejected, this, &SdDBoardBuilder::reject );
  //Help system
  connect( box->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDBoardBuilder.htm"), this );
    } );

  vbox->addWidget( box );

  setLayout( vbox );
  }


void SdDBoardBuilder::accept()
  {
  if( mPlate != nullptr ) {
    //At first we remove previous board region, holes, trace enable region
    SdSelector selector;
    QStringList layerList( {LID0_PCB,LID0_HOLE,LID0_EXCLUSION,LID0_CLEAR} );
    mPlate->forEach( dctLines, [&selector,layerList](SdObject *obj) ->bool {
      //Convert object to Graph object
      SdPtr<SdGraphLinear> ptr(obj);
      if( ptr.isValid() && ptr->isMatchAny(layerList) )
        selector.insert( ptr.ptr() );
      return true; //Continue scan
      } );

    //Delete all selected objects
    SdUndo *undo = mPlate->getUndo();
    undo->begin( QObject::tr("Build board"), mPlate, false );
    selector.forEach( dctAll, [undo] (SdObject *obj) ->bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr )
        graph->deleteObject( undo );
      return true;
      });
    selector.removeAll();

    //Get user input. Bound it for security
    int sizex = qBound( 5000, SdEnvir::instance()->fromPhisPcb( mSizeX->text() ), 500000 );
    int sizey = qBound( 5000, SdEnvir::instance()->fromPhisPcb( mSizeY->text() ), 500000 );
    int holeDiam = qBound( 500, SdEnvir::instance()->fromPhisPcb( mHoleDiam->text() ), 100000 );
    int holeDist = qBound( holeDiam / 2 + 10, SdEnvir::instance()->fromPhisPcb( mHoleDist->text() ), qMin(sizex,sizey) - holeDiam );
    int holeRadius = holeDiam / 2;


    //Build new pcb
    SdPoint p1(0,0);
    SdPoint p2(sizex,sizey);
    SdPropLine pl;
    pl.mType  = dltSolid;
    pl.mWidth = 0;
    pl.mLayer = SdEnvir::instance()->layerGet(LID0_PCB);
    mPlate->insertChild( new SdGraphLinearRect( p1, p2, pl ), undo );

    pl.mLayer = SdEnvir::instance()->layerGet(LID0_HOLE);
    switch( mBoardVariant->checkedId() ) {
      case 2 :
        //Append 2 holes
        p1.set( holeDist, sizey / 2 );
        mPlate->insertChild( new SdGraphLinearCircle( p1, holeRadius, pl ), undo );

        p1.set( sizex - holeDist, sizey / 2 );
        mPlate->insertChild( new SdGraphLinearCircle( p1, holeRadius, pl ), undo );
        break;

      case 6 :
        //Append 6 holes. 2 in middle and 4 at corners
        p1.set( sizex/2, holeDist );
        mPlate->insertChild( new SdGraphLinearCircle( p1, holeRadius, pl ), undo );

        p1.set( sizex/2, sizey - holeDist );
        mPlate->insertChild( new SdGraphLinearCircle( p1, holeRadius, pl ), undo );

        [[fallthrough]];
      case 4 :
        //Append 4 holes
        p1.set( holeDist, holeDist );
        mPlate->insertChild( new SdGraphLinearCircle( p1, holeRadius, pl ), undo );

        p1.set( holeDist, sizey - holeDist );
        mPlate->insertChild( new SdGraphLinearCircle( p1, holeRadius, pl ), undo );

        p1.set( sizex - holeDist, sizey - holeDist );
        mPlate->insertChild( new SdGraphLinearCircle( p1, holeRadius, pl ), undo );

        p1.set( sizex - holeDist, holeDist );
        mPlate->insertChild( new SdGraphLinearCircle( p1, holeRadius, pl ), undo );
        break;
      }
    }
  done(1);
  }
