/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  SdWMain - Main application window
*/

#ifndef SBWMAIN_H
#define SBWMAIN_H

#include "SdConfig.h"
#include "SdWProjectList.h"
#include "SdWEditor.h"
#include "SdWLabel.h"
#include "SdWHelp.h"
#include "guider/SdGuiderStep.h"

#include <QMainWindow>
#include <QSplitter>
#include <QTabWidget>
#include <QStringList>
#include <QClipboard>
#include <QLabel>
#include <QToolButton>

class SdProjectItem;
class SdWProjectTree;
class SdGuiderCapture;
class SdGuiderDialog;
class SdWGuiderRef;

class SdWMain : public QMainWindow
  {
    Q_OBJECT

    SdGuiderCapture *mGuiderCapture; //!< Object for capture guider movie
    SdGuiderDialog  *mGuiderDialog;  //!< Guider scena build dialog
    SdWGuiderRef    *mGuiderRef;     //!< Guider overlay which highlight visual object by its name

    QSplitter       *mWSplitter;     //!< Central part of programm windows - is splitter with projects and redactors
    SdWProjectList  *mWProjectList;  //!< Project list
    QTabWidget      *mWEditors;      //!< Actived visual editors
    SdWHelp         *mWHelp;         //!< Edge help

    //Status bar infos
    SdWLabel        *mXLabel;        //!< X axiz title (X or col)
    SdWLabel        *mXPos;          //!< X axiz position
    SdWLabel        *mYLabel;        //!< Y axiz title (Y or row)
    SdWLabel        *mYPos;          //!< Y axiz position
    SdWLabel        *mTraceStatus;   //!< Current trace status
    SdWLabel        *mMessage;       //!< Message
    QToolButton     *mCapture;       //!< Capture status
    QToolButton     *mRemote;        //!< Remote status

    static SdWMain  *mWMain;         //!< Global pointer to main window
  public:
    explicit SdWMain( QStringList args, QWidget *parent = nullptr );

    //!
    //! \brief instance Returns instance of main window of application
    //! \return         Instance of main window
    //!
    static inline SdWMain *instance() { return mWMain; }

    //!
    //! \brief cmDelegate This template simple translate command to active editor
    //!
    template <void (SdWEditor::*Method)()>
    void cmDelegate()
      {
      //Get active editor ...
      if( SdWEditor *ed = activeEditor() )
        //... and translate command to its method
        (ed->*Method)();
      }

    template <int cmd>
    void cmContextCommand()
      {
      cmContextCommand(cmd);
      }

    void cmContextCommand( int cmd )
      {
      //Get active editor ...
      if( SdWEditor *ed = activeEditor() )
        //... and translate command to its method
        ed->cmContextCommand( cmd );
      }

  signals:

  public slots:
    //Guider
    void highlightWidget( const QString &widgetName );

    //Status bar
    void setStatusLabels( const QString xlabel, const QString ylabel );
    void setStatusPositions( const QString x, const QString y );
    void setStatusMessage( const QString msg );

    //Update tracing status information
    void setTraceStatus( int unconnected, int errors );

    //Show or hide tracing status information
    void showTrace( bool show );

    void activateProjectName( SdProject *project );

    //This signal send from project tree view
    //on it we open appropriate editor
    void onActivateProjectItem( SdProjectItem *item );

    //This signal send from project tree view
    //when user want to open 3d editor
    void onActivateProjectItem3d( SdProjectItem *item );

    void onCloseEditView( SdProjectItem *item );
    void onUpdateItemTitle( SdProjectItem *item );
    void onRemoveProjectItem( SdProjectItem *item );
    void onCloseProject( SdProject *prj );

    //This signal send from project tree when no item selected
    void onActivateProject( SdProject *prj, const QString shortName );

    //This signal send from project tree when project renamed
    void onRenameProject( SdProject *prj, const QString shortName );

    void onClipboardChanged( QClipboard::Mode mode );

    //Browse part in sheet
    void onBrowseSheetPart( SdProjectItem *sheet, SdProjectItem *plate );
    //Components, selected from sheet
    void onSelectedParts( SdProjectItem *plate, QStringList list );



    //Calling when press tab of editor
    void onActivateEditor( int index );
    //Calling when press close tab of editor
    void onCloseEditor( int index );


    //On file received from remote repository
    void onFileReceived(int result, QString fileName, QByteArray data );

    //Commands [Реакции на команды]
    //File menu commands
    void cmFileNew();
    //Load project from library
    void cmFileLoad();
    void cmFileLoadUid( const QString &uid );
    //Load project from local file system
    void cmFileOpen();
    void cmFilePrevious();
    void cmFileClose();
    void cmFileCloseAll();
    void cmFileOpenFile( const QString path );

    void cmFileSave();
    void cmFileSaveAs();
    void cmFileSaveAll();
    void cmFileStore();
    void cmFileCloud();
    void cmFilePrint();
    void cmFileExit();

    void cmFileImportPis();
    void cmFileImport();
    void cmFileExport();

    //Object menu commands
    void cmObjectNew();
    void cmObjectLoad();
    void cmObjectRename();
    void cmObjectDelete();
    void cmObjectCopy();
    void cmObjectPaste();
    void cmObjectCut();
    void cmObjectDuplicate();
    void cmObjectSort();
    void cmObjectParam();
    void cmObjectEditEnable();
    void cmObjectEditDisable();
    void cmObjectPublic();
    void cmProjectParam();
    void cmProjectUpgrade();

    //Edit menu commands
    void cmEditUndo();
    void cmEditRedo();
    void cmEditCut();
    void cmEditCopy();
    void cmEditCopySvg();
    void cmEditCopyWeb();
    void cmEditSelectSave();
    void cmEditPaste();
    void cmEditDelete();
    void cmEditSelectAll();
    void cmEditUnSelect();
    void cmEditFind();
    void cmEditReplace();
    void cmEditProperties();
    void cmEditRotateGroup();
    void cmEditCalculations();
    void cmEditFragments();

    //View menu commands
    void cmViewProject();
    void cmView3d();
    void cmViewFill();
    void cmViewNets();
    void cmViewGrid();
    void cmViewLayers();
    void cmViewZoomIn();
    void cmViewZoomOut();
    void cmViewArea();
    void cmViewMeasurement();
    void cmViewMirrorHorz();
    void cmShowFields( bool st );

    //Common graphics modes
    void cmModeSelect();

    void cmModeBinder();

    void cmModeParam();
    void cmModeScript();

    //3d
    void cm3dMaster();
    void cm3dProgram();
    void cm3dImportStl();
    void cm3dImportStep();
    void cm3dImportVrml();
    void cm3dShow2d();
    void cm3dShowPads();
    void cm3dModeView();
    void cm3dModeHorzMove();
    void cm3dModeHorzRotate();
    void cm3dModeVertMove();
    void cm3dModeVertRotate();
    void cm3dModeFaceColor();

    //Symbol Part
    void cmShowPads(bool st);

    //Sheet and pcb
    void cmRenumeration();
    void cmModeReferenceMove();
    void cmModeValueMove();

    //Sheet
    void cmModeFragment();
    void cmModeComponent();
    void cmModeNet();
    void cmNetSetup();
    void cmModeBus();
    void cmModeDisconnect();
    void cmModePcbArea();
    void cmModeLink();
    void cmModeNetName();
    void cmModeNetList();
    void cmModePack();

    //Pcb
    void cmShowRatNet(bool st);
    void cmShowRuleErrors(bool st);
    void cmPads();
    void cmPcbStratum();
    void cmModeLineSize();
    void cmModeRadiusSize();
    void cmModeMovePart();
    void cmModePartSelect( QStringList list );
    void cmModePartHighlight();

    void cmModeEditWire();
    void cmModeRoadEnter();
    void cmModeRoadMove();
    void cmModePolygon();
    void cmModeTraceDelete();
    void cmModePad();
    void cmModeViaEnter();

    void cmRulesEdit();
    void cmRulesCheck();
    void cmRulesErrorNext();

    void cmOption();
    void cmLibrary();
    void cmRef2LibraryObject();

    void cmHelpIntro();
    void cmHelpPage( const QString topic );
    void cmHelpContents();
    void cmHelpIndex();
    void cmHelpAbout();
    void cmHelpRegistration();
    void cmHelpBackward();
    void cmHelpForward();
    void cmHelpAi();

    void cmGuiderDialog();
    void cmGuiderSnapshotSave();
    void cmGuiderSnapshotLoad(const QString &scriptPath, int snapshotIndex , const SdGuiderStepList&);
    void cmGuiderPlay();
    void cmGuiderCapture();
    void cmGuiderScreenShot();

    void cmRemoteStatus( const QString info );

    void cmClipboardChange();
    void cmPropertiesChange();

    void cmEnterPosition();

    void cmHelpTopic( const QString topic );

    // QWidget interface
  protected:
    virtual void closeEvent(QCloseEvent *ev) override;

  private:
    //Insert editor to tab view
    void            appendEditor(SdWEditor *editor);
    //Get editor by index in tab view
    SdWEditor*      getEditor( int index );
    //Return current active editor
    SdWEditor*      activeEditor();
    //Return help widget in editor area. If none it created
    SdWEditor*      helpWidget();
    //Find help widget in editor area. Return nullptr if none
    SdWEditor*      findHelpWidget();
    //Return current active project view tree
    SdWProjectTree *activeProject() { return mWProjectList->activeProject(); }
    SdWProjectTree *project( int index ) { return mWProjectList->project(index); }
    //Dirty cashe for graphics window and repaint its contents
    void            activeEditorUpdate();
  };

#endif // SBWMAIN_H
