/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdGuiderDialog A dialog widget for displaying and navigating through a scenario structure.

  This class presents a hierarchical view of scenes and steps using a QTreeWidget.
  Scenes (top-level items) are selectable, while steps (child items) are view-only
  and cannot be selected directly. The dialog maintains an always-expanded state
  for all items. It provides navigation methods to track current scene and step
  indices, move to the next step, and handle scene selection events. The dialog
  emits a signal when a scene is selected and includes a build button for
  triggering scenario execution.
*/
#ifndef SDGUIDERDIALOG_H
#define SDGUIDERDIALOG_H

#include "SdGuiderStep.h"

#include <QDialog>
#include <QTreeWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHeaderView>

class SdWMain;


struct SdGuiderScena
  {
    QString          mTitle; //!< Scena title
    SdGuiderStepList mSteps; //!< Scena steps list

    QString hash() const;
  };



class SdGuiderDialog : public QDialog
  {
    Q_OBJECT

    QLabel               *mHintLabel;    //!< Help text
    QTreeWidget          *mTreeWidget;   //!< Scena list widget
    QPushButton          *mBuildButton;  //!< Build button

    QList<SdGuiderScena>  mScenaList;    //!< Scena list
    QTreeWidgetItem      *mCurrentStepItem = nullptr;

    SdWMain              *mWMain;

    QString               mScriptPath;   //!< Path to script
    int                   mScenaIndex;
    int                   mStepIndex;
  public:
    explicit SdGuiderDialog( SdWMain *wmain );

    bool    setScenaFile( const QString &fname );

    QString scriptPath() const { return mScriptPath; }
    int     snapshotIndex() const;

    int     getCurrentScenaIndex() const { return mScenaIndex; }
    int     getCurrentStepIndex() const { return mStepIndex; }
    void    goToNextStep();
    void    clearSelection();

  signals:
    void snapshotLoad( const QString &scriptPath, int snapshotIndex );

  private slots:
    void onItemClicked( QTreeWidgetItem *item, int column );
    void onBuildClicked();
    void onScenaSelected( int index );
  };

#endif // SDGUIDERDIALOG_H
