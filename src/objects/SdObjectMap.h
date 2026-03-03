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

#ifndef SDOBJECTMAP_H
#define SDOBJECTMAP_H

#include <QMap>

class SdObject;

typedef SdObject *SdObjectPtr;

typedef QMap<QString,SdObjectPtr> SdObjectMap;

#endif // SDOBJECTMAP_H
