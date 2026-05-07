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


  }




void SdDNetPinsList::setup(const QString &netName, const SdNetPinRefList &pinRefList)
  {
  mNetName->setCurrentText( netName );
  mPinList = pinRefList;

  //Fill pin table
  for( const auto &pin : std::as_const(mPinList) )
    mPinListWidget->addItem( QString("%1 %2 %3").arg( pin.mPinFunction, pin.mPartImp->ident(), pin.mPinNumber ) );
  }



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
