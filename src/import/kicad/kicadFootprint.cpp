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
#include "../../objects/SdPItemPart.h"
#include "../../objects/SdProject.h"

#include <QObject>

void kicadFootprint(SdScanerKiCad *scaner, SdProject *project )
  {
  //We work with mm
  scaner->ppmSet( 1000.0 );
  QString name;
  if( !scaner->tokenNeedValue( 's', name, QObject::tr("Need footprint name") ) ) return;

  SdPItemPart *part = new SdPItemPart();
  project->insertChild( part, nullptr );

  part->setTitle( name, QObject::tr("Set part title") );

  static QMap<QString,std::function<void( SdScanerKiCad*, SdPItemPart* )> >
      tokenMap( { //{ QString("fp_text"), kicadTextPart },
                  { QString("fp_line"), kicadGraphLine },
                  { QString("fp_circle"), kicadGraphCircle }
                } );

  scaner->parse( tokenMap, part );
  }
