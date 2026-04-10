/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Some common params:
    - interface language
    - update period
*/
#ifndef SDDOPTIONSPAGECOMMON_H
#define SDDOPTIONSPAGECOMMON_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QJsonObject>

class SdDOptionsPageCommon : public QWidget
  {
    Q_OBJECT

    QComboBox   *mLanguage;     //!< Interface language selection
    QSpinBox    *mUpdatePeriod; //!< Update check period in days
    QJsonObject  mUpdateConfig; //!< Update and install configuration
  public:
    explicit SdDOptionsPageCommon(QWidget *parent = nullptr);

  signals:

  public slots:
    void accept();
  };

#endif // SDDOPTIONSPAGECOMMON_H
