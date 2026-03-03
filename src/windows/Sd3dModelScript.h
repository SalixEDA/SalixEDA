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
#ifndef SD3DMODELSCRIPT_H
#define SD3DMODELSCRIPT_H

#include <QString>
#include <QList>

struct Sd3dModelScript
  {
    QString mName;
    QString mDescription;
    QString mScript;

    using Sd3dModelScriptList = QList<Sd3dModelScript>;

    static Sd3dModelScriptList scriptList();
  };

using Sd3dModelScriptList = Sd3dModelScript::Sd3dModelScriptList;


#endif // SD3DMODELSCRIPT_H
