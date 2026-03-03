/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
*/
#include "SdLibraryStorage.h"




//!
//! \brief authorGlobalName Return global author name associated with mAuthorKey (public author key)
//! \return                 Global author name associated with mAuthorKey (public author key)
//!
QString SdLibraryHeader::authorGlobalName() const
  {
  return SdLibraryStorage::instance()->authorGlobalName( mAuthorKey );
  }
