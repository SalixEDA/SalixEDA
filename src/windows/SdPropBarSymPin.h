/*
Project "Electronic schematic and pcb CAD"

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Properties bar for symbol pin
*/
#ifndef SDPROPBARSYMPIN_H
#define SDPROPBARSYMPIN_H

#include "SdPropBarLay.h"
#include "objects/SdPropSymPin.h"

class SdPropBarSymPin : public SdPropBarLay
  {
    Q_OBJECT

    QComboBox *mPinType;  //Symbol pin type
  public:
    SdPropBarSymPin( const QString title );

    void setPropSymPin(const SdPropSymPin &propSymPin );
    void getPropSymPin( SdPropSymPin &propSymPin );

    void setPropSymPin( const SdPropComposerSymPin &propSymPin );
    void getPropSymPin( SdPropComposerSymPin &propSymPin );

  };

#endif // SDPROPBARSYMPIN_H
