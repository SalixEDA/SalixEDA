/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Undo record for part 3d model map throught matrix
*/
#ifndef SDUNDORECORD3DMATRIX_H
#define SDUNDORECORD3DMATRIX_H

#include "SdUndoRecord.h"

#include <QMatrix4x4>

class SdPItemPart;

class SdUndoRecord3dMatrix : public SdUndoRecord
  {
    QMatrix4x4   mMatrix; //!< Conversion matrix
    SdPItemPart *mPart;   //!< Part which object is converted
  public:
    SdUndoRecord3dMatrix( QMatrix4x4 matrix, SdPItemPart *part );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORD3DMATRIX_H
