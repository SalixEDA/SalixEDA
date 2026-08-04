/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Allows select layer pair. Show full layer list
*/
#include "SdDLayerList.h"
#include "ui_SdDLayerList.h"
#include "SdDHelp.h"

#include "objects/SdEnvir.h"

#include <QMessageBox>


SdDLayerList::SdDLayerList(const QString currentPair, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDLayerList)
  {
  ui->setupUi(this);

  connect( ui->mCancel, &QPushButton::clicked, this, &SdDLayerList::cmCancel );
  connect( ui->mAssign, &QPushButton::clicked, this, &SdDLayerList::cmAssign );
  connect( ui->mRemove, &QPushButton::clicked, this, &SdDLayerList::cmRemove );
  connect( ui->mHelp, &QPushButton::clicked, this, [this] () { SdDHelp::help( QStringLiteral("SdDLayerList"), this ); });

  int currentPairIndex = -1, index = 0;
  //Fill layer list
  SdEnvir::instance()->layerForEachConst( dctCommon, [this, &currentPairIndex, &index, currentPair] (SdLayer *layer) -> bool {
    //Append id to internal list
    mLayerIdList.append( layer->id() );
    if( layer->id() == currentPair )
      currentPairIndex = index;
    index++;
    //Create visual representation of layer name
    QString name;
    if( layer->pair() != nullptr && layer->pair() != layer )
      //Marker shows layer already has pair
      name = QString("(*) ");
    name.append( layer->name() );
    ui->mList->addItem( name );
    return true;
    });

  if( currentPairIndex >= 0 )
    ui->mList->setCurrentRow( currentPairIndex );

  // Assign unique names for the interactive help system
  ui->mList->setObjectName("SdDLayerList.mList");
  ui->mAssign->setObjectName("SdDLayerList.mAssign");
  ui->mRemove->setObjectName("SdDLayerList.mRemove");
  }




SdDLayerList::~SdDLayerList()
  {
  delete ui;
  }




//Returned selected layer pair
QString SdDLayerList::pair() const
  {
  int selected = ui->mList->currentRow();
  if( selected >= 0 && selected < mLayerIdList.count() )
    //Selection present
    return mLayerIdList.at(selected);
  return QString();
  }




//Nothing done
void SdDLayerList::cmCancel()
  {
  done(SdDLayerList_cancel);
  }



//Remove current pair
void SdDLayerList::cmRemove()
  {
  done(SdDLayerList_remove);
  }



//Assign new pair
void SdDLayerList::cmAssign()
  {
  //Check if selection present
  int selected = ui->mList->currentRow();
  if( selected >= 0 && selected < mLayerIdList.count() )
    done(SdDLayerList_assign);
  else
    QMessageBox::warning( this, tr("Error"), tr("Pair not selected to assign. Select it and repeate assigning.") );
  }



