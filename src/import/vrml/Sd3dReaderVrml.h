/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  3d object reader for VRML files
*/
#ifndef SD3DREADERVRML_H
#define SD3DREADERVRML_H

#include "objects/Sd3dGraphModel.h"

class Sd3dReaderVrml
  {
  public:

    //!
    //! \brief importVrml Read model from VRML file which represented by its path
    //! \param fname      Full path to VRML file
    //! \param parent     Parent widget. Is used to display messages and progress bar
    //! \return           Pointer to Sd3dGraph object if import was successfull or nullptr in other case
    //!
    static Sd3dGraph   *importVrmlFromFile( QString fname, QWidget *parent );

  };

#endif // SD3DREADERVRML_H
