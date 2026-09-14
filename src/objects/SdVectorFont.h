/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdVectorFont - vector font parser and render
*/
#ifndef SDVECTORFONT_H
#define SDVECTORFONT_H

#include "SdPropText.h"

#include <QChar>
#include <QMap>
#include <QLine>
#include <QString>
#include <QVector>

//! \brief Class for loading and storing custom vector font data.
class SdVectorFont {
    QMap<QChar, QVector<QLine>> mGlyphMap; //!< Container holding glyph mappings from character to its vector graphics.
public:
  /// \brief Constructor that loads font from file.
  /// \param fileName Path to the font definition file.
  explicit SdVectorFont(const QString &fileName);

  //!
  //! \brief stringPath Generates a vector of lines for a transformed text string.
  //! \param str        The input text string to render.
  //! \param prop       Text properties
  //! \return           Vector of transformed QLine segments.
  //!
  QVector<QLine> stringPath( const QString &str, const SdPropText &prop );

private:
  /// \brief Parses the vector font file.
  /// \param fileName Path to the font definition file.
  void loadFromFile(const QString &fileName);

};


#endif // SDVECTORFONT_H
