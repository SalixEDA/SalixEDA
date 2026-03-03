/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Interface language definition
  Table of supported interface languages
*/
#ifndef SDLANGUAGE_H
#define SDLANGUAGE_H

#include <QString>
#include <QList>

struct SdLanguage
  {
    QString mTitle; //Language title (aca English)
    QString mId;    //Language id (aca en)

    SdLanguage( const QString title, const QString id );

    //Table of languages
    typedef QList<SdLanguage> SdLanguageList;

    //Table of supported languages
    static SdLanguageList languageList();
  };

#endif // SDLANGUAGE_H
