/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Dialog for library management
*/
#ifndef SDDLIBRARY_H
#define SDDLIBRARY_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

class SdDLibrary : public QDialog
  {
    Q_OBJECT

    QLineEdit *mServerRepo;           //!< Remote library server ip
    QLineEdit *mLibraryPath;          //!< Local Library path [Каталог библиотек]
    //QLineEdit *mCloudLibraryPath;
  public:
    SdDLibrary();
  };

#endif // SDDLIBRARY_H
