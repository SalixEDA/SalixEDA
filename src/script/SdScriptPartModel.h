/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Special variable. When we assign to it then model appended to the PartItem.
*/
#ifndef SDSCRIPTPARTMODEL_H
#define SDSCRIPTPARTMODEL_H

#include "SdScriptValueVariable.h"

class SdPItemPart;
class Sd3dGraphModel;

class SdScriptPartModel : public SdScriptValueVariable
  {
    Sd3drModel *mModel; //!< Model which accumulate all faces
  public:
    SdScriptPartModel( Sd3drModel *model );

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char type() const override { return SD_SCRIPT_TYPE_MODEL; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief isWriteOnly Return true for special write-only variables with undefined value when read
    //! \return            true for special write-only variables with undefined value when read
    //!
    virtual bool isWriteOnly() const override { return true; }

    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void assign(SdScriptValuePtr src) override;
  };

#endif // SDSCRIPTPARTMODEL_H
