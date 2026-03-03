/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Rich text object editor.
*/
#ifndef SDWEDITORRICH_H
#define SDWEDITORRICH_H

#include "SdWEditor.h"
#include "objects/SdPItemRich.h"

#include <QTextEdit>

class SdWEditorRich : public SdWEditor
  {
    Q_OBJECT

    SdPItemRich  *mRich;
    QTextEdit    *mTextEdit;
    bool          mDirty;
  public:
    SdWEditorRich(SdPItemRich *item, QWidget *parent = nullptr );
    ~SdWEditorRich() override;

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;

  public slots:
    //On text changed
    void onTextChanged();
  };

#endif // SDWEDITORRICH_H
