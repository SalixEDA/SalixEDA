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

#include "SdGuiderDialog.h"
#include "SdGuiderCapture.h"
#include "SvLib/SvDir.h"
#include "windows/SdWMain.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QCryptographicHash>
#include <QSettings>
#include <QMessageBox>
#include <QTemporaryFile>

QString SdGuiderScena::hash() const {
  QCryptographicHash hasher( QCryptographicHash::Blake2s_128 );

  // Add scene title to hash
  hasher.addData( mTitle.toUtf8() );

  // Add step count to control structure integrity
  hasher.addData( QByteArray::number( mSteps.size() ) );

  // Add each step to hash
  for( const auto &step : std::as_const(mSteps) ) {
    // Add step text
    hasher.addData( step.mTitle.toUtf8() );

    // Add step duration
    hasher.addData( QByteArray::number( step.mDuration ) );

    // Add separator for reliability (to avoid collisions
    // during concatenation, e.g. "ab"+"c" vs "a"+"bc")
    hasher.addData( "|" );
    }

  // Return result as hex string
  return QString::fromLatin1( hasher.result().toHex() );
  }







//!
//! \brief SdGuiderDialog Constructor
//! \param wmain          Pointer to the main application window
//!
SdGuiderDialog::SdGuiderDialog(SdWMain *wmain )
  : QDialog( nullptr )
  , mWMain(wmain) {
  // Create main layout
  QVBoxLayout *mainLayout = new QVBoxLayout( this );

  // Create and configure QLabel for hint
  mHintLabel = new QLabel( this );
  mHintLabel->setWordWrap( true );
  mHintLabel->setStyleSheet( "QLabel { background-color: #f0f0f0; padding: 5px; }" );
  mainLayout->addWidget( mHintLabel );

  //Current titer
  mCurrentStepTiter = new QLabel( this );
  mCurrentStepTiter->setWordWrap( true );
  mainLayout->addWidget( mCurrentStepTiter );

  // Create and configure QTreeWidget
  mTreeWidget = new QTreeWidget( this );
  mTreeWidget->setHeaderLabel( tr("Script") );
  mTreeWidget->setAlternatingRowColors( true );
  mTreeWidget->setIndentation( 20 );

  // Disable collapsing (all items always expanded)
  mTreeWidget->setItemsExpandable( false );
  mTreeWidget->setExpandsOnDoubleClick( false );

  // Allow selection of scenes only (top-level items)
  mTreeWidget->setSelectionMode( QAbstractItemView::SingleSelection );

  // Configure appearance
  QHeaderView *header = mTreeWidget->header();
  header->setStretchLastSection( true );
  header->setSectionResizeMode( QHeaderView::ResizeToContents );

  mainLayout->addWidget( mTreeWidget );

  QHBoxLayout *buttons = new QHBoxLayout();
  // Create Build button
  QPushButton *but = new QPushButton( tr("Build"), this );
  but->setToolTip( tr("Builds full movie from all scenas") );
  but->setMinimumHeight( 30 );
  connect( but, &QPushButton::pressed, this, &SdGuiderDialog::onBuildClicked );
  buttons->addWidget( but );

  but = new QPushButton( tr("Play"), this );
  but->setToolTip( tr("Start play current scena") );
  but->setMinimumHeight( 30 );
  connect( but, &QPushButton::pressed, this, [this] () {
    if( mScenaIndex < mScenaList.size() ) {
      onScenaSelected( mScenaIndex );
      mWMain->setFocus();
      mWMain->cmGuiderPlay();
      }
    });
  buttons->addWidget( but );

  but = new QPushButton( tr("Capture"), this );
  but->setToolTip( tr("Start capture current scena") );
  but->setMinimumHeight( 30 );
  connect( but, &QPushButton::pressed, this, [this] () {
    if( mScenaIndex < mScenaList.size() ) {
      onScenaSelected( mScenaIndex );
      mWMain->setFocus();
      mWMain->cmGuiderCapture();
      }
    });
  buttons->addWidget( but );


  but = new QPushButton( tr("Shots"), this );
  but->setToolTip( tr("Start shots capture current scena") );
  but->setMinimumHeight( 30 );
  connect( but, &QPushButton::pressed, this, [this] () {
    if( mScenaIndex < mScenaList.size() ) {
      onScenaSelected( mScenaIndex );
      mWMain->setFocus();
      mWMain->cmGuiderScreenShot();
      }
    });
  buttons->addWidget( but );


  but = new QPushButton( tr("Shots size"), this );
  but->setToolTip( tr("Resize main window for shots") );
  but->setMinimumHeight( 30 );
  connect( but, &QPushButton::pressed, this, [this] () {
    //Made fixed window size
    mWMain->resize( 1080, 640 );
    });
  buttons->addWidget( but );

  mainLayout->addLayout( buttons );

  // Configure dialog properties
  setWindowTitle( tr("The script") );
  setMinimumSize( 800, 200 );

  // Connect signals
  connect( mTreeWidget, &QTreeWidget::itemClicked, this, &SdGuiderDialog::onItemClicked );
  }







//!
//! \brief setScenaFile Loads and sets the scena file
//! \param fname        Path to the scena file
//! \return             true if file loaded successfully, false otherwise
//!
bool SdGuiderDialog::setScenaFile(const QString &fname)
  {
  QFileInfo info(fname);
  SvDir dir(info.absolutePath());
  mScriptPath = dir.slashedPath();
  mScriptName = info.baseName();

  QFile file(fname);
  if( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
    qDebug() << "Cannot open file:" << fname;
    return false;
    }

  QTextStream in(&file);
  QString currentLine;

  SdGuiderScena currentScena;
  SdGuiderStep currentStep;
  QString currentStepText;
  bool inStep = false;

  mScenaList.clear();

  while( !in.atEnd() ) {
    currentLine = in.readLine();

    // Remove extra spaces at the beginning and end, but preserve internal spaces
    QString trimmedLine = currentLine.trimmed();

    // Skip empty lines
    if( trimmedLine.isEmpty() )
      continue;

    // Check line start
    if( trimmedLine.startsWith("--") ) {
      // This is a step (starts with two dashes)

      // Save previous step if it existed
      if( inStep ) {
        currentStep.mTitle = currentStepText.trimmed();
        currentScena.mSteps.append(currentStep);
        currentStepText.clear();
        }

      // Remove two dashes from the beginning
      QString stepContent = trimmedLine.mid(2).trimmed();

      // Create a new step
      currentStep = SdGuiderStep();
      currentStep.mDuration = 3; // Default value

      // Check if there is a number after the dash (duration)
      int spacePos = stepContent.indexOf(' ');
      if (spacePos > 0) {
        // Try to parse the number at the beginning
        QString firstPart = stepContent.left(spacePos);
        bool ok;
        int duration = firstPart.toInt(&ok);
        if (ok) {
          // Found a number - this is the duration
          currentStep.mDuration = duration;
          // The rest of the line is the step text
          currentStepText = stepContent.mid(spacePos + 1);
          }
        else {
          // No number - the entire line is the step text
          currentStepText = stepContent;
          }
        }
      else {
        // No space - possibly just a number or just text
        bool ok;
        int duration = stepContent.toInt(&ok);
        if (ok) {
          // Only a number - this is the duration without text
          currentStep.mDuration = duration;
          currentStepText = "";
          }
        else {
          // Only text without duration
          currentStepText = stepContent;
          }
        }

      inStep = true;
      }
    else if( trimmedLine.startsWith("-") ) {
      // This is a scene title (starts with a single dash)

      // Save previous scene if it's not empty
      if( !currentScena.mTitle.isEmpty() || !currentScena.mSteps.isEmpty() ) {
        // Save the last step of the current scene
        if( inStep ) {
          currentStep.mTitle = currentStepText.trimmed();
          currentScena.mSteps.append(currentStep);
          currentStepText.clear();
          inStep = false;
          }
        mScenaList.append(currentScena);
        }

      // Start a new scene
      currentScena = SdGuiderScena();
      // Remove one dash from the beginning
      currentScena.mTitle = trimmedLine.mid(1).trimmed();
      currentScena.mSteps.clear();
      inStep = false;
      }
    else {
      // This is a continuation of the previous step (multi-line text)
      if( inStep ) {
        if( !currentStepText.isEmpty() )
          currentStepText += " ";
        currentStepText += trimmedLine;
        }
      else {
        // Line without marker, but not in a step - ignore or add to title?
        // According to the specification, such lines should not exist
        qDebug() << "Warning: Line without marker outside step:" << trimmedLine;
        }
      }
    }

  // Save the last step if it existed
  if( inStep ) {
    currentStep.mTitle = currentStepText.trimmed();
    currentScena.mSteps.append(currentStep);
    }

  // Save the last scene
  if( !currentScena.mTitle.isEmpty() || !currentScena.mSteps.isEmpty() ) {
    mScenaList.append(currentScena);
    }

  file.close();

  mTreeWidget->clear();

  QTreeWidgetItem *scenaFirst = nullptr;

  // Fill the tree
  for( const auto &scena : std::as_const(mScenaList) ) {
    // Create scene item (top level)
    QTreeWidgetItem *scenaItem = new QTreeWidgetItem( mTreeWidget );
    if( scenaFirst == nullptr ) scenaFirst = scenaItem;
    scenaItem->setText( 0, scena.mTitle );
    scenaItem->setData( 0, Qt::UserRole, "scena" );

    // Disable collapsing of this item
    scenaItem->setFlags( scenaItem->flags() & ~Qt::ItemIsUserCheckable );

    // Add steps
    for( const auto &step : std::as_const(scena.mSteps) ) {
      QTreeWidgetItem *stepItem = new QTreeWidgetItem( scenaItem );

      // Format step text with duration
      QString stepText = step.mTitle;
      if( !stepText.isEmpty() )
        stepText = QString( tr("[%1 sec] %2") ).arg( step.mDuration ).arg( step.mTitle );
      else
        stepText = QString( tr("[%1 sec]") ).arg( step.mDuration );

      stepItem->setText( 0, stepText );
      stepItem->setData( 0, Qt::UserRole, "step" );

      // Steps cannot be selected
      stepItem->setFlags( stepItem->flags() & ~Qt::ItemIsSelectable );
      }
    }

  // Expand all items
  mTreeWidget->expandAll();

  // Update hint
  mHintLabel->setText( "F12 - save snapshot, F11 - record start-stop, F10 - next step" );

  //Set the first scene as active
  mScenaIndex = mStepIndex = 0;
  mTreeWidget->setCurrentItem( scenaFirst );

  return true;
  }


//!
//! \brief snapshotIndex Returns the current snapshot index
//! \return              Snapshot index
//!
int SdGuiderDialog::snapshotIndex() const
  {
  int scena = getCurrentScenaIndex();
  if( scena >= 0 ) {
    int step = getCurrentStepIndex();
    if( step == 0 ) return scena;
    return scena + 1;
    }
  return -1;
  }
















//!
//! \brief stepIndexChanged Called when step index changes
//! \param stepIndex        New step index
//!
void SdGuiderDialog::stepIndexChanged(int stepIndex)
  {
  if( mScenaIndex < mScenaList.size() ) {
    const SdGuiderScena &scena = mScenaList.at(mScenaIndex);
    if( stepIndex < scena.mSteps.size() ) {
      mStepIndex = stepIndex;
      mCurrentStepTiter->setText( QString("%1.").arg(mStepIndex+1) + scena.mSteps.at(mStepIndex).mTitle );
      }
    else {
      mCurrentStepTiter->setText( tr("All steps completed") );
      }
    }
  }





//!
//! \brief onItemClicked Handles tree widget item click
//! \param item          Clicked tree widget item
//! \param column        Column index of the click
//!
void SdGuiderDialog::onItemClicked( QTreeWidgetItem *item, int column ) {
  Q_UNUSED( column );

  if( !item ) return;

  // Check if the item is a scene
  if( item->data( 0, Qt::UserRole ).toString() == "scena" ) {
    // Find the scene index
    int index = mTreeWidget->indexOfTopLevelItem( item );
    if( index >= 0 ) {
      mScenaIndex = index;
      // Clear step selection when selecting a new scene
      //clearSelection();

      // Emit signal
      onScenaSelected( index );

      qDebug() << "Scene selected:" << index << item->text( 0 );
      }
    }
  else {
    // If a step was attempted to be selected, deselect it
    item->setSelected( false );
    }
  }





//!
//! \brief onBuildClicked Handles build button click
//!
void SdGuiderDialog::onBuildClicked() {

  // Create a temporary file with the list
  QTemporaryFile listFile;
  if( !listFile.open() )
    return;

  // Write the list of files
  QTextStream stream(&listFile);
  for( int sceneIndex = 0; sceneIndex < mScenaList.size(); ++sceneIndex ) {
    //At first we try file with audio
    //Full path to the scene file considering the language
    QString path = SdGuiderCapture::moviePath( mScriptPath, sceneIndex, true );
    //Check if it exists, then add it to the list
    if( QFile::exists( path ) )
      stream << "file '" << path << "'\n";
    else {
      //Try file without audio
      path = SdGuiderCapture::moviePath( mScriptPath, sceneIndex, false );
      //Check if it exists, then add it to the list
      if( QFile::exists( path ) )
        stream << "file '" << path << "'\n";
      }
    }
  listFile.close();


  // Output file
  QSettings s;
  QString outputPath = mScriptPath + mScriptName + QString("-%1.mp4").arg( s.value(SDK_LANGUAGE).toString() );

  // Remove existing output file
  QFile::remove(outputPath);

  // FFmpeg arguments
  QStringList args;
  args << "-f" << "concat"
       << "-safe" << "0"
       << "-i" << listFile.fileName()
       << "-c:v" << "copy"
       << "-filter_complex" << "aresample=async=1"
       << outputPath;

  // Run FFmpeg
  QProcess::execute("ffmpeg", args);
  }



//!
//! \brief onScenaSelected Handles scena selection change
//! \param index           Index of the selected scena
//!
void SdGuiderDialog::onScenaSelected(int index)
  {
  mScenaIndex = index;
  stepIndexChanged( 0 );
  emit snapshotLoad( mScriptPath, mScenaIndex, mScenaList.at( mScenaIndex ).mSteps );
  }
