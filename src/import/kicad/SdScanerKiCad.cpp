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

SdScanerKiCad::SdScanerKiCad()
  {

  }



void SdScanerKiCad::parseTop(SdProject *project)
  {
  //Token map
  static QMap<QString,std::function<void( SdScanerKiCad*, SdProject* )> >
      tokenMap( { { QString("footprint"), kicadFootprint },
                  { QString("kicad_symbol_lib"), kicadSymbolLib }
                } );

  //Parse full file
  tokenNext();
  while( !isEndOfScanOrError() )
    parseContent( tokenMap, project );
  }






void SdScanerKiCad::tokenNext()
  {
  mToken = 0;
  while( !isEndOfScanOrError() ) {
    blank();
    if( !isEndOfScanOrError() ) {
      if( mLine.at(mIndex).isLetter() ) {
        mToken = 'n';
        scanName();
        }
      else if( mLine.at(mIndex).isDigit() || mLine.at(mIndex) == QChar('-') ) {
        scanDouble( true, false );
        mToken = mTokenValue.contains( QChar('.') ) ? 'f' : 'd';
        }
      else if( mLine.at(mIndex) == QChar('"') ) {
        //String detected
        mToken = 's';
        scanString( QChar('"'), QChar('\\'), QStringLiteral("Unclosed string") );
        }
      else mToken = mLine.at(mIndex++).toLatin1();
      break;
      }
    }
  }
