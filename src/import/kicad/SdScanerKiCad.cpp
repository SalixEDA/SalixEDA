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
#include "SdScanerKiCad.h"

#include <QObject>



void SdScanerKiCad::parseTop(SdProject *project)
  {
  mDestProject = project;

  }



void SdScanerKiCad::footprint(SdScanerSe *)
  {

  }




void SdScanerKiCad::symbolLib(SdScanerSe *)
  {

  }






