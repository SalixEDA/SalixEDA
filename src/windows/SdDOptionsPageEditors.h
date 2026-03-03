/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Some editor parametrs
*/
#ifndef SDDOPTIONSPAGEEDITORS_H
#define SDDOPTIONSPAGEEDITORS_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>

class SdDOptionsPageEditors : public QWidget
  {
    Q_OBJECT

    QCheckBox *mShowRatNet;
    QCheckBox *mShowRemarks;
    QCheckBox *mShowMessage;
    QCheckBox *mCenterCursor;

    QLineEdit *mSymbolPinSize;
    QLineEdit *mPartPinSize;
    QLineEdit *mWireDotSize;
    QLineEdit *mWireDotWidth;
    QLineEdit *mViaType;
    QLineEdit *mWidthStep;
    QLineEdit *mTextSizeStep;
    QLineEdit *mMinViewGridSize;
    QLineEdit *mCursorViewSize;
    QLineEdit *mTraceDotSize;
  public:
    explicit SdDOptionsPageEditors(QWidget *parent = nullptr);

  signals:

  public slots:
    void accept();
  };

#endif // SDDOPTIONSPAGEEDITORS_H
