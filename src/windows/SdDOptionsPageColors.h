/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Display and selecting system colors
*/

#ifndef SDDOPTIONSPAGECOLORS_H
#define SDDOPTIONSPAGECOLORS_H

#include <QWidget>
#include <QTableWidget>

class SdDOptionsPageColors : public QWidget
  {
    Q_OBJECT

    QTableWidget *mTable; //Table with colors
  public:
    explicit SdDOptionsPageColors(QWidget *parent = nullptr);

  signals:

  public slots:
    //Activate on select cell in color table
    //We shows color dialog
    void onColorSelect( int row, int column );

    void accept();

  private:
    void addColor(int colorIndex, const QString descr);
  };

#endif // SDDOPTIONSPAGECOLORS_H
