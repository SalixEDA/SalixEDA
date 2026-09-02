/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Properties bar for part pin
*/
#ifndef SDPROPBARPARTPIN_H
#define SDPROPBARPARTPIN_H

#include "SdPropBarLay.h"
#include "objects/SdPropPartPin.h"

class SdPropBarPartPin : public SdPropBarLay
  {
    Q_OBJECT

    QComboBox   *mPinSide;          //Part pin side (top, bottom or through)
    QComboBox   *mPinType;          //Part pin type
  public:
    SdPropBarPartPin( const QString title );

    void setPropPartPin( const SdPropPartPin &propPartPin );
    void getPropPartPin( SdPropPartPin &propPartPin );

    void setPropPartPin( const SdPropComposerPartPin &propPartPin );
    void getPropPartPin( SdPropComposerPartPin &propPartPin );

    //!
    //! \brief mBarId Prop Bar id
    //!
    static constexpr int mBarId = PB_PART_PIN;

  private slots:
    void setPinType();
  };


using SdPropBarPartPinPtr = SdPropBarPointer<SdPropBarPartPin>;

#endif // SDPROPBARPARTPIN_H
