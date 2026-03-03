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

  Array of values. It is base for value arrays some types
*/
#ifndef SDSCRIPTVALUEARRAY_H
#define SDSCRIPTVALUEARRAY_H

#include "SdScriptValue.h"

class SdScriptValueArray : public SdScriptValue
  {
  protected:
    SdScriptValuePtrList mArray; //!< Array of pointers to value
  public:
    SdScriptValueArray() : SdScriptValue() {}
    ~SdScriptValueArray() { qDeleteAll(mArray); }

    //!
    //! \brief append Appends value to array
    //! \param val    Appended value
    //!
    void         append( SdScriptValuePtr val ) { mArray.append( val ); }

    //!
    //! \brief typeOfElement Return type of element of array
    //! \return              Type of element of array
    //!
    virtual char typeOfElement() const = 0;
  };

#endif // SDSCRIPTVALUEARRAY_H
