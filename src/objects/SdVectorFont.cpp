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
#include "SdVectorFont.h"

#include "SdVectorFont.h"
#include <QFile>
#include <QTextStream>

SdVectorFont::SdVectorFont(const QString &fileName)
  {
  loadFromFile(fileName);
  }




//!
//! \brief stringPath Generates a vector of lines for a transformed text string.
//! \param str        The input text string to render.
//! \param prop       Text properties
//! \return           Vector of transformed QLine segments.
//!
QVector<QLine> SdVectorFont::stringPath(const QString &str, const SdPropText &prop)
  {
  QVector<QLine> rawPath;
  int currentX = 0;
  const int fontHeight = 10;
  const int spaceWidth = 4;
  const int charSpacing = 1;

  // Phase 1: Accumulate unscaled font segments along the X-axis
  for( int i = 0; i < str.length(); ++i ) {
    QChar ch = str.at(i);
    int charWidth = 0;

    if( ch == QChar(' ') )
      charWidth = spaceWidth;
    else if( mGlyphMap.contains(ch) ) {
      const QVector<QLine> &glyphLines = mGlyphMap.value(ch);

      // Find the bounding width of the current glyph
      int maxGlyphX = 0;
      for( const QLine &line : glyphLines ) {
        if( line.x1() > maxGlyphX ) { maxGlyphX = line.x1(); }
        if( line.x2() > maxGlyphX ) { maxGlyphX = line.x2(); }
        }
      charWidth = maxGlyphX;

      // Shift and append the glyph's lines to the raw path
      for( const QLine &line : glyphLines ) {
        rawPath.append(QLine(line.x1() + currentX, line.y1(),
                             line.x2() + currentX, line.y2()));
        }
      }

    // Advance X position for the next character
    currentX += charWidth;
    if( i < str.length() - 1 )
      currentX += charSpacing;
    }

  int totalWidth = currentX;

  // Phase 2: Compute alignment offsets in the local coordinate system
  double offsetX = 0.0;
  double offsetY = 0.0;

  // Horizontal alignment
  if( prop.mHorz.value() == dhjCenter )
    offsetX = -totalWidth / 2.0;
  else if( prop.mHorz.value() == dhjRight )
    offsetX = -totalWidth;


  // Vertical alignment
  if( prop.mVert.value() == dvjMiddle )
    offsetY = -fontHeight / 2.0;
  else if( prop.mVert.value() == dvjTop )
    offsetY = -fontHeight;


  // Phase 3: Construct the transformation matrix using QTransform
  QTransform transform;

  // 3a. Translate for alignment offsets
  transform.translate(offsetX, offsetY);

  // 3b. Apply horizontal mirroring if requested
  if( prop.mMirror.value() )
    transform.scale(-1.0, 1.0);


  // 3c. Apply scale factor (size dynamic resolution)
  double scaleFactor = prop.mSize.value() / 10.0;
  transform.scale(scaleFactor, scaleFactor);

  // 3d. Apply rotation around the alignment origin point
  double degrees = prop.mDir.getDegree();
  transform.rotate(degrees);

  // Phase 4: Transform all segments into the final coordinate space
  QVector<QLine> transformedPath;
  transformedPath.reserve( rawPath.size() );

  for( const QLine &line : rawPath )
    transformedPath.append( transform.map(line) );


  return transformedPath;
  }








void SdVectorFont::loadFromFile(const QString &fileName)
  {
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
    }

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine().trimmed();

    // Skip empty lines and comments starting with space or semicolon
    if( line.isEmpty() || line.startsWith(';') || line.startsWith(' ') ) {
      continue;
      }

    if( line.startsWith(QLatin1String("Symbol ")) ) {
      // Parse the symbol line definition
      QString definition = line.mid(7).trimmed();
      if( definition.isEmpty() ) {
        continue;
        }

      QChar currentTargetChar = definition.at(0);

      //Check if this symbol is an alias for another symbol
      int sameAsIndex = definition.indexOf(QLatin1String("sameAs"));
      if( sameAsIndex != -1 ) {
        QString aliasPart = definition.mid(sameAsIndex + 6).trimmed();
        if( !aliasPart.isEmpty() ) {
          mGlyphMap[currentTargetChar] = mGlyphMap[aliasPart.at(0)];
          }
        }
      else {
        //Get next line and parse
        line = in.readLine().trimmed();

        auto tokens = line.tokenize(QChar(' '));
        QVector<int> values;
        for( const auto &token : tokens ) {
          bool ok = false;
          int val = token.toInt(&ok);
          if (ok) {
            values.append(val);
            }
          }

        // Process integer point pairs
        QVector<QLine> path;
        QPoint p1;
        bool isMove = true;
        for( int i = 0; i + 1 < values.size(); i += 2 ) {
          if( isMove ) {
            p1 = QPoint(p1.x() + values[i], p1.y() + values[i + 1]);
            isMove = false;
            }
          else if( values[i] == 0 && values[i + 1] == 127 )
            isMove = true;
          else {
            QPoint p2( p1.x() + values[i], p1.y() + values[i + 1] );
            path.append( QLine(p1,p2) );
            p1 = p2;
            }
          }
        mGlyphMap[currentTargetChar] = path;
        }
      }
    }

  file.close();
  }
