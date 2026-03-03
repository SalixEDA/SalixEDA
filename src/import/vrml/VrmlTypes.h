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
#ifndef VRMLTYPES_H
#define VRMLTYPES_H

#include <QString>
#include <QList>

using VrmlFloat  = float;
using VrmlInt32  = int;
using VrmlBool   = bool;
using VrmlString = QString;

using VrmlInt32List = QList<VrmlInt32>;

//Forward scaner declaration
class SdScanerVrml;

#endif // VRMLTYPES_H
