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
#ifndef IGESPARAMETERDATA_H
#define IGESPARAMETERDATA_H

#include <QByteArray>
#include <QMap>

class IgesParameterData
  {
    QByteArray mData;
  public:
    IgesParameterData();

    void appendData( const QByteArray &ar ) { mData += ar; }
  };

using IgesParameterDataMap = QMap<int,QByteArray>;

#endif // IGESPARAMETERDATA_H
