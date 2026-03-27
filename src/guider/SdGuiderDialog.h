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

    QLabel               *mHintLabel;        //!< Help text
    QLabel               *mCurrentStepTiter; //!< Text of current step
    QTreeWidget          *mTreeWidget;       //!< Scena list widget

    QList<SdGuiderScena>  mScenaList;        //!< Scena list
    SdWMain              *mWMain;            //!< Root window of application

    QString               mScriptPath;       //!< Path to script
    QString               mScriptName;       //!< Name of script
    int                   mScenaIndex;       //!< Current scena index
    int                   mStepIndex;        //!< Current step index in current scena
  public:
    //!
    //! \brief SdGuiderDialog Constructor
    //! \param wmain          Pointer to the main application window
    //!
    explicit SdGuiderDialog( SdWMain *wmain );

    //!
    //! \brief setScenaFile Loads and sets the scena file
    //! \param fname        Path to the scena file
    //! \return             true if file loaded successfully, false otherwise
    //!
    bool    setScenaFile( const QString &fname );

    //!
    //! \brief scriptPath Returns the current script path
    //! \return           Path to the script
    //!
    QString scriptPath() const { return mScriptPath; }

    //!
    //! \brief snapshotIndex Returns the current snapshot index
    //! \return              Snapshot index
    //!
    int     snapshotIndex() const;

    //!
    //! \brief getCurrentScenaIndex Returns the current scena index
    //! \return                     Current scena index
    //!
    int     getCurrentScenaIndex() const { return mScenaIndex; }

    //!
    //! \brief getCurrentStepIndex Returns the current step index
    //! \return                    Current step index
    //!
    int     getCurrentStepIndex() const { return mStepIndex; }

  signals:
    //!
    //! \brief snapshotLoad  Emitted when a snapshot needs to be loaded
    //! \param scriptPath    Path to the script
    //! \param snapshotIndex Index of the snapshot
    //! \param list          List of steps for the snapshot
    //!
    void    snapshotLoad( const QString &scriptPath, int snapshotIndex, const SdGuiderStepList &list );

  public slots:
    //!
    //! \brief stepIndexChanged Called when step index changes
    //! \param stepIndex        New step index
    //!
    void    stepIndexChanged( int stepIndex );

  private slots:
    //!
    //! \brief onItemClicked Handles tree widget item click
    //! \param item          Clicked tree widget item
    //! \param column        Column index of the click
    //!
    void onItemClicked( QTreeWidgetItem *item, int column );

    //!
    //! \brief onBuildClicked Handles build button click
    //!
    void onBuildClicked();

    //!
    //! \brief onScenaSelected Handles scena selection change
    //! \param index           Index of the selected scena
    //!
    void onScenaSelected( int index );
  };
#endif // SDGUIDERDIALOG_H
