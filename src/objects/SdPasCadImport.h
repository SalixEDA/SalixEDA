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
#ifndef SDPASCADIMPORT_H
#define SDPASCADIMPORT_H

#include "SdProject.h"
#include "library/SdStringMap.h"

#include <QFile>
#include <QWidget>
#include <QMap>
#include <QList>

struct DLayer;
struct DFont;
class  SdLayer;
class  SdPropText;
class  SdPItemComponent;
class  SdGraphIdent;
class  SdRect;

typedef QMap<int,SdObjectPtr> SdImportMap;

using SdListOfStringMap = QList<SdStringMap>;

using SdPasCadPinMap = QMap<QString,SdListOfStringMap>;

class SdPasCadImport
  {
    struct SdPasCadComponentInfo {
        QString mComponentHash; //!< SalixEDA component hash
        int     mSymbolIndex;   //!< Symbol index in object map
        int     mPartIndex;     //!< Part index in object map
      };
    using SdPasCadCompList = QList<SdPasCadComponentInfo>;

    SdProject         *mProject;
    QFile             *mFile;
    QWidget           *mOwner;
    DLayer            *mLayerTable;
    int                mLayerNumber;
    DFont             *mFontTable;
    int                mFontNumber;
    SdImportMap        mObjectMap;
    SdListOfStringMap  mPinsPack;      //!< List of sections pins. Each section is association pin name with pin number
    SdPasCadPinMap     mSymbolsPinMap; //!< Pin assignment for each symbol
    SdPasCadCompList   mComponentList; //!< Component info list
  public:
    SdPasCadImport( SdProject *prj, QWidget *owner );
    ~SdPasCadImport();

    //Import entry point
    bool import( const QString fname );

  private:
    bool         project();
    bool         projectItem( SdProjectItem *item );
    bool         readContainer( SdProjectItem *item );
    SdObject    *buildObject( int id );
    SdLayer     *readLayer();

    int          readInt8();
    int          readInt16();
    int          readInt32();
    bool         readBool();
    QString      readNString();
    QString      readConstString( int len );
    QString      readWName() { return readConstString(32); }
    QString      readName() { return readConstString(8); }
    bool         error( QString msg );
    bool         readObjectTable( SdContainer *container );
    bool         readSingleObject( SdContainer *container );
    bool         readSymbol( SdObject *obj );
    bool         readPart( SdObject *obj );
    bool         readSheet( SdObject *obj );
    SdPoint      readPoint();
    SdRect       readRectangle();
    bool         readLinear( SdObject *obj );
    bool         readRect( SdObject *obj );
    bool         readLine( SdObject *obj );
    bool         readTextProp(SdPropText *prp , SdPoint *origin);
    bool         readText( SdObject *obj );
    bool         readCircle( SdObject *obj );
    bool         readArc( SdObject *obj );
    bool         readSymPin( SdObject *obj );
    bool         readIdent( SdGraphIdent *ident );
    bool         readPartPin( SdObject *obj );
    bool         readComponent( SdObject *obj );
  };

#endif // SDPASCADIMPORT_H
