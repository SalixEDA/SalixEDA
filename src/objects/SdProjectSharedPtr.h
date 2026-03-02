/*
Project "Electronic schematic and pcb CAD"

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Shared pointer to SdProject
*/
#ifndef SDPROJECTSHAREDPTR_H
#define SDPROJECTSHAREDPTR_H

#include <QSharedPointer>

class SdProject;

typedef QSharedPointer<SdProject> SdProjectSharedPtr;

#endif // SDPROJECTSHAREDPTR_H
