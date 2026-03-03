/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdScanerMultyline - Append to scaner function to scan from multyline source, such as files
*/
#include "SdScanerMultyline.h"

#include <QFile>

SdScanerMultyline::SdScanerMultyline() :
  mLineIndex(0)
  {

  }




//!
//! \brief sourceSetStringList Set source lines as ready-made line list
//! \param lines               Ready-made line list
//!
void SdScanerMultyline::sourceSetStringList(QStringList lines)
  {
  mLines = lines;
  mLineIndex = 0;
  mEndOfScan = false;
  }




//!
//! \brief sourceSetString Set source lines as single line delimited eoln symbols
//! \param lines           Source lines as single line delimited eoln symbols
//!
void SdScanerMultyline::sourceSetString(QString lines)
  {
  sourceSetStringList( lines.split(QChar('\n')) );
  }





//!
//! \brief read Read previously opened file contents to scaner
//! \param file Previously opened file
//! \return     true - if reading successfull
//!
bool SdScanerMultyline::read(QIODevice *file)
  {
  sourceSetString( QString::fromUtf8( file->readAll() ) );
  return true;
  }




//!
//! \brief readFile Read file contents to scaner
//! \param path     Full path to file
//! \return         true - if reading successfull
//!
bool SdScanerMultyline::readFile(const QString &path)
  {
  QFile file(path);
  if( file.open( QIODevice::ReadOnly ) ) {
    return read( &file );
    }
  return false;
  }



//!
//! \brief nextLine Take next line from source
//! \return         true if there is next line
//!
bool SdScanerMultyline::nextLine()
  {
  if( mLineIndex < mLines.count() ) {
    lineSet( mLines.at(mLineIndex++) );
    return true;
    }
  return false;
  }
