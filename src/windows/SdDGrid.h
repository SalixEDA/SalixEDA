/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Grid options dialog
*/
#ifndef SDDGRID_H
#define SDDGRID_H

#include <QDialog>

namespace Ui {
  class SdDGrid;
}

class SdDGrid : public QDialog
  {
    Q_OBJECT

    QPointF mGrid; //Current grid
  public:
    explicit SdDGrid( QPointF curGrid, QWidget *parent = nullptr );
    ~SdDGrid() override;

    QPointF getGrid() const { return mGrid; }

  private:
    Ui::SdDGrid *ui;

    //Fill X and Y fields
    void printGrid( QPointF g );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDGRID_H
