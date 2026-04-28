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

#include "SdDMasterSymbolIcWithNum.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QClipboard>
#include <QGuiApplication>


static QString strDelim("---");
static QString strGap;


static int cellHeight    = 500;
static int pinSizeX      = 750;
static int nameAreaX     = 1250;
static int extendedAreaX = 3750;
static int centralSizeX  = 1500;


SdDMasterSymbolIcWithNum::SdDMasterSymbolIcWithNum(SdProjectItem *item, QWidget *parent)
  : SdDMasterSymbol( item, parent )
  {
  QHBoxLayout *hcentral = new QHBoxLayout();
  QHBoxLayout *hbox;
  QVBoxLayout *vbox;
  QPushButton *but;

  vbox = new QVBoxLayout();
  vbox->addWidget( new QLabel(tr("Left pins")) );
  vbox->addWidget( mLeftTable = new QTableWidget() );
  hbox = new QHBoxLayout();

  but = new QPushButton( tr("Insert pin (Ctrl+L)") );
  connect( but, &QPushButton::clicked, this, &SdDMasterSymbolIcWithNum::leftInsertPin );
  but->setShortcut( QString("Ctrl+L") );
  hbox->addWidget( but );

  but = new QPushButton( tr("Insert gap") );
  connect( but, &QPushButton::clicked, this, &SdDMasterSymbolIcWithNum::leftInsertGap );
  hbox->addWidget( but );

  but = new QPushButton( tr("Insert delim") );
  connect( but, &QPushButton::clicked, this, &SdDMasterSymbolIcWithNum::leftInsertDelimiter );
  hbox->addWidget( but );


  but = new QPushButton( tr("Delete") );
  connect( but, &QPushButton::clicked, this, &SdDMasterSymbolIcWithNum::leftDelete );
  hbox->addWidget( but );

  vbox->addLayout( hbox );

  hcentral->addLayout( vbox );


  vbox = new QVBoxLayout();
  vbox->addWidget( new QLabel(tr("Source pins")) );
  vbox->addWidget( mCentralTable = new QTableWidget() );
  hbox = new QHBoxLayout();

  but = new QPushButton( tr("Move left") );
  connect( but, &QPushButton::clicked, this, &SdDMasterSymbolIcWithNum::moveLeft );
  hbox->addWidget( but );

  but = new QPushButton( tr("From clipbrd") );
  connect( but, &QPushButton::clicked, this, &SdDMasterSymbolIcWithNum::fromClipboard );
  hbox->addWidget( but );

  but = new QPushButton( tr("Move right") );
  connect( but, &QPushButton::clicked, this, &SdDMasterSymbolIcWithNum::moveRight );
  hbox->addWidget( but );

  vbox->addLayout( hbox );

  hcentral->addLayout( vbox );


  vbox = new QVBoxLayout();
  vbox->addWidget( new QLabel(tr("Right pins")) );
  vbox->addWidget( mRightTable = new QTableWidget() );
  hbox = new QHBoxLayout();

  but = new QPushButton( tr("Insert pin (Ctrl+R)") );
  connect( but, &QPushButton::clicked, this, &SdDMasterSymbolIcWithNum::rightInsertPin );
  but->setShortcut( QString("Ctrl+R") );
  hbox->addWidget( but );

  but = new QPushButton( tr("Insert gap") );
  connect( but, &QPushButton::clicked, this, &SdDMasterSymbolIcWithNum::rightInsertGap );
  hbox->addWidget( but );

  but = new QPushButton( tr("Insert delim") );
  connect( but, &QPushButton::clicked, this, &SdDMasterSymbolIcWithNum::rightInsertDelimiter );
  hbox->addWidget( but );


  but = new QPushButton( tr("Delete") );
  connect( but, &QPushButton::clicked, this, &SdDMasterSymbolIcWithNum::rightDelete );
  hbox->addWidget( but );

  vbox->addLayout( hbox );

  hcentral->addLayout( vbox );



  vbox = new QVBoxLayout();
  vbox->addWidget( new QLabel(tr("Preview")) );
  vbox->addWidget( mPreview = new QLabel() );

  hcentral->addLayout( vbox );



  hbox = new QHBoxLayout();
  hbox->addWidget( new QLabel(tr("Title:")) );
  hbox->addWidget( mTitle = new QLineEdit() );
  auto *dbox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
  hbox->addWidget( dbox );

  connect( dbox, &QDialogButtonBox::accepted, this, &SdDMasterSymbolIcWithNum::accept );
  connect( dbox, &QDialogButtonBox::rejected, this, &SdDMasterSymbolIcWithNum::reject );

  vbox = new QVBoxLayout();
  vbox->addLayout( hcentral );
  vbox->addLayout( hbox );
  setLayout( vbox );



  mLeftTable->setColumnCount(3);
  mLeftTable->setHorizontalHeaderLabels( {tr("Pin name"), tr("Pin num"), tr("Alternative names")} );
  mLeftTable->setColumnWidth( 0, 100 );
  mLeftTable->setColumnWidth( 1, 100 );
  mLeftTable->setColumnWidth( 2, 150 );
  mLeftTable->setSelectionMode( QAbstractItemView::SingleSelection );

  mRightTable->setColumnCount(3);
  mRightTable->setHorizontalHeaderLabels( {tr("Pin name"), tr("Pin num"), tr("Alternative names")} );
  mRightTable->setColumnWidth( 0, 100 );
  mRightTable->setColumnWidth( 1, 100 );
  mRightTable->setColumnWidth( 2, 150 );
  mRightTable->setSelectionMode( QAbstractItemView::SingleSelection );

  mCentralTable->setColumnCount(2);
  mLeftTable->setHorizontalHeaderLabels( {tr("Pin name"), tr("Pin num")} );
  mLeftTable->setColumnWidth( 0, 100 );
  mLeftTable->setColumnWidth( 1, 100 );
  mLeftTable->setSelectionMode( QAbstractItemView::SingleSelection );

  connect( mLeftTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIcWithNum::onPinChanged );
  connect( mRightTable, &QTableWidget::cellChanged, this, &SdDMasterSymbolIcWithNum::onPinChanged );


  onPinChanged(0,0);
  }



void SdDMasterSymbolIcWithNum::onPinChanged(int row, int column)
  {
  Q_UNUSED(row)
  Q_UNUSED(column)

  SdIllustrator il;

  drawSymbol( il );
  il.setPixmap( 400, 600, Qt::white );
  drawSymbol( il );
  mPreview->setPixmap( il.pixmap() );
  }



void SdDMasterSymbolIcWithNum::leftInsertPin()
  {
  QString pinName;
  if( mLeftTable->currentRow() >= 0 )
    pinName = nextText( mLeftTable->item(mLeftTable->currentRow(),0)->text() );
  leftInsert( pinName, QString{} );
  }



void SdDMasterSymbolIcWithNum::leftInsertGap()
  {
  leftInsert( strGap, QString{} );
  }

void SdDMasterSymbolIcWithNum::leftInsertDelimiter()
  {
  leftInsert( strDelim, QString{} );
  }

void SdDMasterSymbolIcWithNum::leftDelete()
  {
  int cr = mLeftTable->currentRow();
  if( cr >= 0 ) {
    QString pinName = mLeftTable->item( cr, 0 )->text();
    QString pinNumber = mLeftTable->item( cr, 1 )->text();
    mLeftTable->removeRow(cr);
    cr = mCentralTable->rowCount();
    mCentralTable->insertRow( cr );
    mCentralTable->setItem( cr, 0, new QTableWidgetItem(pinName) );
    mCentralTable->setItem( cr, 1, new QTableWidgetItem(pinNumber) );
    onPinChanged( 0, 0 );
    }
  }





void SdDMasterSymbolIcWithNum::rightInsertPin()
  {
  QString pinName;
  if( mRightTable->currentRow() >= 0 )
    pinName = nextText( mRightTable->item(mRightTable->currentRow(),0)->text() );
  rightInsert( pinName, QString{} );
  }

void SdDMasterSymbolIcWithNum::rightInsertGap()
  {
  rightInsert( strGap, QString{} );
  }

void SdDMasterSymbolIcWithNum::rightInsertDelimiter()
  {
  rightInsert( strDelim, QString{} );
  }

void SdDMasterSymbolIcWithNum::rightDelete()
  {
  int cr = mRightTable->currentRow();
  if( cr >= 0 ) {
    QString pinName = mRightTable->item( cr, 0 )->text();
    QString pinNumber = mRightTable->item( cr, 1 )->text();
    mRightTable->removeRow(cr);
    cr = mCentralTable->rowCount();
    mCentralTable->insertRow( cr );
    mCentralTable->setItem( cr, 0, new QTableWidgetItem(pinName) );
    mCentralTable->setItem( cr, 1, new QTableWidgetItem(pinNumber) );
    onPinChanged( 0, 0 );
    }
  }





void SdDMasterSymbolIcWithNum::moveLeft()
  {
  int cr = mCentralTable->currentRow();
  if( cr >= 0 ) {
    QString pinName = mCentralTable->item( cr, 0 )->text();
    QString pinNumber = mCentralTable->item( cr, 1 )->text();
    mCentralTable->removeRow( cr );
    mCentralTable->setCurrentCell( cr, 0 );
    leftInsert( pinName, pinNumber );
    }
  }

void SdDMasterSymbolIcWithNum::moveRight()
  {
  int cr = mCentralTable->currentRow();
  if( cr >= 0 ) {
    QString pinName = mCentralTable->item( cr, 0 )->text();
    QString pinNumber = mCentralTable->item( cr, 1 )->text();
    mCentralTable->removeRow( cr );
    mCentralTable->setCurrentCell( cr, 0 );
    rightInsert( pinName, pinNumber );
    }
  }




void SdDMasterSymbolIcWithNum::fromClipboard()
  {
  QClipboard *clip = QGuiApplication::clipboard();
  QString str = clip->text();
  if( !str.isEmpty() ) {
    QStringList list = str.split( QChar('\n') );
    if( list.count() ) {
      for( const QString &line : std::as_const(list) ) {
        if( !line.isEmpty() ) {
          QStringList lineList = line.split( QChar(',') );
          QString pinName;
          QString pinNumber;
          if( lineList.count() )
            pinName = lineList.at(0).simplified();
          if( lineList.count() > 1 )
            pinNumber = lineList.at(1).simplified();
          int cr = mCentralTable->rowCount();
          mCentralTable->insertRow( cr );
          mCentralTable->setRowHeight( cr, 20 );
          mCentralTable->setItem( cr, 0, new QTableWidgetItem(pinName) );
          mCentralTable->setItem( cr, 1, new QTableWidgetItem(pinNumber) );
          mCentralTable->setCurrentCell( cr, 0 );
          }
        }
      }
    }
  }




void SdDMasterSymbolIcWithNum::drawSymbol(SdIllustrator &il)
  {
  //Cell count on left and right sides
  int leftCellCount = mLeftTable->rowCount();
  int rightCellCount = mRightTable->rowCount();
  //Calc extending pin name presentation
  bool extended = false;
  for( int i = 0; i < leftCellCount && !extended; i++ )
    extended = !mLeftTable->item( i, 2 )->text().isEmpty();
  for( int i = 0; i < rightCellCount && !extended; i++ )
    extended = !mRightTable->item( i, 2 )->text().isEmpty();

  int maxPins     = qMax(leftCellCount,rightCellCount);
  if( maxPins < 1 ) maxPins = 1;
  //SdEnvir::instance()->fromPhisSchematic( ui->mCellHeight->text() );
  //SdEnvir::instance()->fromPhisSchematic( ui->mPinSizeX->text() );
  //int nameAreaX   = 1250;
  int cellSizeX   = nameAreaX;
  if( extended ) cellSizeX += extendedAreaX; //SdEnvir::instance()->fromPhisSchematic( ui->mCellSizeX->text() );
  int leftPin     = 0;
  int leftBody    = pinSizeX;
  int rightBody   = leftBody + 2 * cellSizeX + centralSizeX;
  int rightPin    = rightBody + pinSizeX;
  int topBody     = cellHeight / 2;
  int bottomBody  = topBody - maxPins * cellHeight;


  QColor red("red");
  QColor green("green");

  //Body
  il.drawRect( leftBody, topBody,  rightBody, bottomBody, red );
  //Vertical delimiter
  int leftDelimiter = leftBody + cellSizeX;
  il.drawLine( leftDelimiter, topBody,  leftDelimiter, bottomBody, red );
  int rightDelimiter = leftDelimiter + centralSizeX;
  il.drawLine( rightDelimiter, topBody,  rightDelimiter, bottomBody, red );

  //Pins
  int nameSize = 300;
  for( int i = 0; i < leftCellCount; i++ ) {
    QString pinName = mLeftTable->item( i, 0 )->text();
    QString pinNumber = mLeftTable->item( i, 1 )->text();
    if( pinName.isEmpty() ) {
      //This is gap. Nothing done
      }
    else if( pinName == strDelim ) {
      //Delimiter
      il.drawLine( pinSizeX, i * -cellHeight, leftDelimiter, i * -cellHeight, red );
      }
    else {
      //Pin line
      il.drawLine( leftPin, i * -cellHeight, pinSizeX, i * -cellHeight, red );
      //Pin crosshair
      il.drawCross( leftPin, i * -cellHeight, 50, green );
      //Pin name
      il.drawTextMapped( pinSizeX + 10, i * -cellHeight - 150, pinName, red, nameSize );
      //Pin number
      if( !pinNumber.isEmpty() )
        il.drawTextMapped( leftPin + 2, i * -cellHeight + 150, pinNumber, red, nameSize );
      }
    }

  for( int i = 0; i < rightCellCount; i++ ) {
    QString pinName = mRightTable->item( i, 0 )->text();
    QString pinNumber = mRightTable->item( i, 1 )->text();
    if( pinName.isEmpty() ) {
      //This is gap. Nothing done
      }
    else if( pinName == strDelim ) {
      //Delimiter
      il.drawLine( rightBody, i * -cellHeight, rightDelimiter, i * -cellHeight, red );
      }
    else {
      //Pin line
      il.drawLine( rightBody, i * -cellHeight, rightPin, i * -cellHeight, red );
      //Pin crosshair
      il.drawCross( rightPin, i * -cellHeight, 50, green );
      //Pin name
      il.drawTextMapped( rightBody - 1240, i * -cellHeight - 150, pinName, red, nameSize );
      //Pin number
      if( !pinNumber.isEmpty() )
        il.drawTextMapped( rightBody + 2, i * -cellHeight + 150, pinNumber, red, nameSize );
      }
    }
  }



static void separateStringToTwoLines( const QString &src, QString &fl, QString &sec )
  {
  QStringList list = src.split( QChar('/') );
  int firstLine = 0;
  int firstLineWidth = 0;
  while( firstLineWidth < 36 ) {
    if( fl.length() ) fl.append( QString("/") );
    fl.append( list.at(firstLine) );
    firstLineWidth += list.at(firstLine).length() + 1;
    firstLine++;
    }
  //Remaining names append to second line
  while( firstLine < list.count() ) {
    if( sec.length() ) sec.append( QString("/") );
    sec.append( list.at(firstLine++) );
    }
  }




void SdDMasterSymbolIcWithNum::leftInsert(const QString &pinName, const QString &pinNumber)
  {
  int cr = mLeftTable->currentRow();
  if( cr < 0 )
    cr = mLeftTable->rowCount();
  else
    cr++;
  disconnect( mLeftTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIcWithNum::onPinChanged );
  mLeftTable->insertRow( cr );
  mLeftTable->setRowHeight( cr, 20 );
  mLeftTable->setItem( cr, 0, new QTableWidgetItem(pinName) );
  mLeftTable->setItem( cr, 1, new QTableWidgetItem(pinNumber) );
  mLeftTable->setItem( cr, 2, new QTableWidgetItem() );
  mLeftTable->setCurrentCell( cr, 0 );
  connect( mLeftTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIcWithNum::onPinChanged );
  onPinChanged( 0, 0 );
  mLeftTable->setFocus();
  }

void SdDMasterSymbolIcWithNum::rightInsert(const QString &pinName, const QString &pinNumber)
  {
  int cr = mRightTable->currentRow();
  if( cr < 0 )
    cr = mRightTable->rowCount();
  else
    cr++;
  disconnect( mRightTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIcWithNum::onPinChanged );
  mRightTable->insertRow( cr );
  mRightTable->setRowHeight( cr, 20 );
  mRightTable->setItem( cr, 0, new QTableWidgetItem(pinName) );
  mRightTable->setItem( cr, 1, new QTableWidgetItem(pinNumber) );
  mRightTable->setItem( cr, 2, new QTableWidgetItem() );
  mRightTable->setCurrentCell( cr, 0 );
  connect( mRightTable,  &QTableWidget::cellChanged, this, &SdDMasterSymbolIcWithNum::onPinChanged );
  onPinChanged( 0, 0 );
  mRightTable->setFocus();
  }





void SdDMasterSymbolIcWithNum::addAttr(int x, int y, int attr)
  {
  switch( attr ) {
    case 1 :
      //Circle at root
      addCircle( x, y, 50 );
      break;
    case 2 :
      //Cross at root
      addLine( x - 50, y - 50, x + 50, y + 50 );
      addLine( x + 50, y - 50, x - 50, y + 50 );
      break;
    case 3 :
      //Slash /
      addLine( x - 100, y - 100, x + 100, y + 100 );
      break;
    case 4 :
      //Back slash
      addLine( x + 100, y - 100, x - 100, y + 100 );
      break;
    }
  }





bool SdDMasterSymbolIcWithNum::addSymbol(const QString &pinName, int x, int y)
  {
  if( pinName == QString("(Z)") ) {
    //Romb
    addLine( x, y - 100, x + 50, y );
    addLine( x + 50, y, x, y + 100 );
    addLine( x, y + 100, x - 50, y );
    addLine( x - 50, y, x, y - 100 );
    addLine( x - 50, y, x + 50, y );
    return true;
    }
  if( pinName == QString("(OK)") ) {
    //Romb
    addLine( x, y - 100, x + 50, y );
    addLine( x + 50, y, x, y + 100 );
    addLine( x, y + 100, x - 50, y );
    addLine( x - 50, y, x, y - 100 );
    addLine( x - 50, y + 100, x + 50, y + 100 );
    return true;
    }
  if( pinName == QString("(OE)") ) {
    //Romb
    addLine( x, y - 100, x + 50, y );
    addLine( x + 50, y, x, y + 100 );
    addLine( x, y + 100, x - 50, y );
    addLine( x - 50, y, x, y - 100 );
    addLine( x - 50, y - 100, x + 50, y - 100 );
    return true;
    }
  return false;
  }






QPair<QString,int> extractAttr( const QString &pinName )
  {
  if( pinName.length() > 2 ) {
    if( pinName.startsWith("o ") )
      return QPair<QString,int>( pinName.mid(2), 1 );
    if( pinName.startsWith("x ") )
      return QPair<QString,int>( pinName.mid(2), 2 );
    if( pinName.startsWith("/ ") )
      return QPair<QString,int>( pinName.mid(2), 3 );
    if( pinName.startsWith("\\ ") )
      return QPair<QString,int>( pinName.mid(2), 4 );
    }
  return QPair<QString,int>( pinName, 0 );
  }



void SdDMasterSymbolIcWithNum::accept()
  {
  //Cell count on left and right sides
  int leftCellCount = mLeftTable->rowCount();
  int rightCellCount = mRightTable->rowCount();
  //Calc extending pin name presentation
  bool extended = false;
  for( int i = 0; i < leftCellCount && !extended; i++ )
    extended = !mLeftTable->item( i, 2 )->text().isEmpty();
  for( int i = 0; i < rightCellCount && !extended; i++ )
    extended = !mRightTable->item( i, 2 )->text().isEmpty();

  int maxPins     = qMax(leftCellCount,rightCellCount);
  if( maxPins < 1 ) maxPins = 1;
  int cellSizeX   = nameAreaX;
  if( extended ) cellSizeX += extendedAreaX; //SdEnvir::instance()->fromPhisSchematic( ui->mCellSizeX->text() );
  int leftPin      = 0;
  int leftBody     = pinSizeX;
  int rightBody    = leftBody + 2 * cellSizeX + centralSizeX;
  int rightPin     = rightBody + pinSizeX;
  int topBody      = cellHeight / 2;
  int bottomBody   = topBody - maxPins * cellHeight;
  int leftPinName  = leftBody + nameAreaX/2;
  int rightPinName = rightBody - nameAreaX/2;
  int leftExtended = leftBody + nameAreaX + 10;


  //Body
  addRect( leftBody, topBody,  rightBody, bottomBody );
  //Vertical delimiter
  int leftDelimiter = leftBody + cellSizeX;
  addLine( leftDelimiter, topBody,  leftDelimiter, bottomBody );
  int rightDelimiter = leftDelimiter + centralSizeX;
  addLine( rightDelimiter, topBody,  rightDelimiter, bottomBody );

  //Properties for extended pin names
  SdPropText ptn;
  ptn = mPinNameProp;
  ptn.mSize = ptn.mSize.value() / 2;
  ptn.mHorz = dhjLeft;

  //Pins

  mPinNumberProp.mHorz = dhjLeft;
  mPinNameProp.mHorz = dhjCenter;
  for( int i = 0; i < leftCellCount; i++ ) {
    QString pinName = mLeftTable->item( i, 0 )->text();
    QString pinNumber = mLeftTable->item( i, 1 )->text();
    if( pinName.isEmpty() ) {
      //This is gap. Nothing done
      }
    else if( pinName == strDelim ) {
      //Delimiter
      addLine( pinSizeX, i * -cellHeight, leftDelimiter, i * -cellHeight );
      }
    else if( !addSymbol( pinName, leftPinName, i * -cellHeight ) ) {
      //Pin line
      addLine( leftPin, i * -cellHeight, pinSizeX, i * -cellHeight );
      //Pin
      auto [pinNameSim,attr] = extractAttr( pinName );
      SdPoint pinOrg(leftPin, i * -cellHeight);
      addPin( pinOrg, 0, SdPoint(leftPinName,pinOrg.y()), pinNameSim, SdPoint(leftPin,pinOrg.y()+250), pinNumber );
      addAttr( leftBody, i * -cellHeight, attr );
      //Addon pin names separated /
      QString an = mLeftTable->item( i, 2 )->text();
      if( !an.isEmpty() ) {
        if( an.length() < 36 ) {
          //Display as single string
          qDebug() << "one line" << an;
          addText( leftExtended, i * -cellHeight, ptn, an );
          }
        else {
          //Display as two string
          //We split line to separated names
          // then, we append to first line names while their summarized width less then 36
          QString fl, sec;
          separateStringToTwoLines( an, fl, sec );
          qDebug() << "two lines" << fl << sec;
          //Display two lines
          addText( leftExtended, i * -cellHeight +  cellHeight / 4, ptn, fl );
          addText( leftExtended, i * -cellHeight -  cellHeight / 4, ptn, sec );
          }
        }
      }
    }

  mPinNumberProp.mHorz = dhjRight;
  for( int i = 0; i < rightCellCount; i++ ) {
    QString pinName = mRightTable->item( i, 0 )->text();
    QString pinNumber = mRightTable->item( i, 1 )->text();
    if( pinName.isEmpty() ) {
      //This is gap. Nothing done
      }
    else if( pinName == strDelim ) {
      //Delimiter
      addLine( rightBody, i * -cellHeight, rightDelimiter, i * -cellHeight );
      }
    else if( !addSymbol( pinName, rightPinName, i * -cellHeight ) ) {
      //Pin line
      addLine( rightBody, i * -cellHeight, rightPin, i * -cellHeight );
      //Pin
      auto [pinNameSim,attr] = extractAttr( pinName );
      SdPoint pinOrg(rightPin, i * -cellHeight);
      addPin( pinOrg, 0, SdPoint(rightPinName,pinOrg.y()), pinNameSim, SdPoint(rightPin,pinOrg.y()+250), pinNumber );
      addAttr( rightBody, i * -cellHeight, attr );
      //Addon pin names separated /
      QString an = mRightTable->item( i, 2 )->text();
      if( !an.isEmpty() ) {
        if( an.length() < 36 ) {
          //Display as single string
          addText( rightDelimiter + 10, i * -cellHeight, ptn, an );
          }
        else {
          //Display as two string
          //We split line to separated names
          // then, we append to first line names while their summarized width less then 36
          QString fl, sec;
          separateStringToTwoLines( an, fl, sec );
          //Display two lines
          addText( rightDelimiter + 10, i * -cellHeight +  cellHeight / 4, ptn, fl );
          addText( rightDelimiter + 10, i * -cellHeight -  cellHeight / 4, ptn, sec );
          }
        }
      }
    }

  //id
  int hcenter = (leftDelimiter + rightDelimiter) / 2;
  setId( SdPoint( hcenter, topBody + cellHeight/2 ) );

  //value
  setValue( SdPoint( hcenter, bottomBody - cellHeight/2 ) );

  //Symbol title
  SdPropText pt;
  pt.mHorz  = dhjCenter;
  pt.mVert  = dvjMiddle;
  pt.mSize  = mPinNumberProp.mSize;
  pt.mLayer = mLineProp.mLayer;
  addText( hcenter, topBody - cellHeight/2, pt, mTitle->text() );


  SdDMasterSymbol::accept();
  }
