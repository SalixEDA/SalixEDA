/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Library
*/
#ifndef SDDOPTIONSPAGELIBRARY_H
#define SDDOPTIONSPAGELIBRARY_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>

class SdDOptionsPageLibrary : public QWidget
  {
    Q_OBJECT
    QLineEdit *mLocalLibraryPath;     //!< Local Library path [Каталог библиотек]
    QLineEdit *mGlobalStorageIp;      //!< Global storage ip
    QLineEdit *mPrivateCloudIp;       //!< Private cloud ip
    QLineEdit *mPrivateCloudId;       //!< Private cloud id

    QLineEdit *mAuthorName;           //!< Author name
    QLineEdit *mObjectsCount;         //!< Objects count in library
    QCheckBox *mAutoUpload;           //!< Enable automatic upload objects into remote library

  public:
    explicit SdDOptionsPageLibrary(QWidget *parent = nullptr);

  signals:

  public slots:
    void accept();
  };

#endif // SDDOPTIONSPAGELIBRARY_H
