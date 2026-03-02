/*
Project "Electronic schematic and pcb CAD"

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
