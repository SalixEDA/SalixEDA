/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Part implementation properties
*/

#ifndef SDPARTIMPPROP_H
#define SDPARTIMPPROP_H

#include "SdPvAngle.h"
#include "SdPvStratum.h"
#include "SdJsonIO.h"
#include "objects/SdPvMulty.h"

//!
//! \brief The SdPropPartImp struct - Part implementation properties
//!        Stores rotation angle and board side placement for component parts
//!
struct SdPropPartImp
  {
    SdPvAngle   mAngle;  //!< Angle of rotation component [Угол поворота компонента]
    SdPvStratum mSide;   //!< Side of component location [Сторона расположения на плате]

    //!
    //! \brief json Function to write object content into json writer
    //! \param js   Json writer
    //!
    void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Function to read object content from json reader
    //! \param js   Json reader
    //!
    void json( const SdJsonReader &js);

    //!
    //! \brief swap Swap all part implementation properties with another instance
    //! \param other Other SdPropPartImp object to swap with
    //!
    void swap( SdPropPartImp &other );
  };


using SdPropComposerPartImp = SdPropComposer<SdPropPartImp, &SdPropPartImp::mAngle, &SdPropPartImp::mSide>;

#endif // SDPARTIMPPROP_H
