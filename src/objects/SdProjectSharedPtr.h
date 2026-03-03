/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

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
