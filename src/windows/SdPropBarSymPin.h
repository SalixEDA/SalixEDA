/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

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

    //!
    //! \brief mBarId Prop Bar id
    //!
    static constexpr int mBarId = PB_SYM_PIN;
  };

using SdPropBarSymPinPtr = SdPropBarPointer<SdPropBarSymPin>;

#endif // SDPROPBARSYMPIN_H
