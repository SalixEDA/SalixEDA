/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Part imp information: origin position, side and angle of rotation
*/
#ifndef SDPARTIMPINFO_H
#define SDPARTIMPINFO_H

#include "SdPoint.h"
#include "SdPropPartImp.h"

struct SdPartImpInfo
  {
    SdPoint       mOrigin; //!< Part imp origin position
    SdPropPartImp mProp;   //!< Part imp side and angle of rotation
    bool          mValid;  //!< Flag of valid information

    SdPartImpInfo() : mValid(false) {}
    SdPartImpInfo( const SdPoint &org, const SdPropPartImp &prp ) : mOrigin(org), mProp(prp), mValid(true) {}
  };


#endif // SDPARTIMPINFO_H
