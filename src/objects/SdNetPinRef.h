/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdNetPinRef - reference to single pin in part imp for textual representation of net pin list.
*/
#ifndef SDNETPINREF_H
#define SDNETPINREF_H

#include "SdJsonIO.h"

#include <QMap>
#include <QList>



class SdGraphPartImp;

struct SdNetPinRef
  {
    SdGraphPartImp *mPartImp;     //!< Part implement of pin
    QString         mPinNumber;   //!< Pin number
    QString         mPinFunction; //!< Textual representation for pin function (aka VCC, VDD, GND)

    bool operator < ( const SdNetPinRef &pin ) const { return mPinFunction < pin.mPinFunction; }

    //!
    //! \brief isEqual Compares pin refs. Return true if pins are equals
    //! \param pin     Pin to compare with
    //! \return        true if pins are equals
    //!
    bool        isEqual( const SdNetPinRef &pin ) const { return mPartImp == pin.mPartImp && mPinNumber == pin.mPinNumber; }

    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void        json( SdJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void        json( const SdJsonReader &js );

  };

using SdNetPinRefMap = QMap<QString,SdNetPinRef>;

using SdNetPinRefList = QList<SdNetPinRef>;

#endif // SDNETPINREF_H
