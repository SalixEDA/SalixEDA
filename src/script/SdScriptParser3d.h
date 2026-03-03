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
#ifndef SDSCRIPTPARSER3D_H
#define SDSCRIPTPARSER3D_H

#include "SdScriptParser.h"

class SdScriptParser3d : public SdScriptParser
  {
  public:
    SdScriptParser3d(QTableWidget *tableWidget, Sd3drModel *model);

    //!
    //! \brief parse   Execute parsing of programm source text and generates tree of programm
    //! \param src     Programm source text
    //! \param part    Part to which will be placed generated faces
    //! \return        Programm tree
    //!
    SdScriptProgrammPtr parse3d(const QString src, SdPItemPart *part, Sd3drModel *model);
  };

#endif // SDSCRIPTPARSER3D_H
