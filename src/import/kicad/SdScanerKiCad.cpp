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
#include "SdKiCad.h"

#include <QObject>



void SdScanerKiCad::parseTop(SdProject *project)
  {
  //Token map
  static SdSeTokenMap
      tokenMap( { { QString("footprint"), kicadFootprint },
                  { QString("kicad_symbol_lib"), kicadSymbolLib }
                } );

  //Parse full file
  tokenNext();
  while( !isEndOfScanOrError() )
    parseContent( tokenMap, project );
  }






