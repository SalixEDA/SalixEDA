/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  This file contains list of all gui commands and toolBars
*/

#ifndef SDWCOMMAND_H
#define SDWCOMMAND_H

#include "SdConfig.h"
#include "modes/SdModeIdents.h"
#include "objects/SdClass.h"
#include <QAction>
#include <QMenu>
#include <QToolBar>

typedef QAction *QActionPtr;
typedef QToolBar *QToolBarPtr;

class SdWMain;
class SdPropBar;
class SdProjectItem;
using SdPropBarPtr = SdPropBar*;

class SdWCommand
  {
  public:
    //static QMenu *
    static QMenu *menuFile;
    static QMenu *menuFilePrevious;
    static QMenu *menuObject;
    static QMenu *menuEdit;
    static QMenu *menuView;
    static QMenu *menuViewPart3d;
    static QMenu *menuDraw;
    static QMenu *menuInsertSymbol;
    static QMenu *menuInsertSheet;
    static QMenu *menuInsertPart;
    static QMenu *menuInsertPart3d;
    static QMenu *menuInsertPcb;
    static QMenu *menuInsertComp;
    static QMenu *menuInstruments;
    static QMenu *menuRules;
    static QMenu *menuHelp;

    static QMenu *menuSelect;
    static QMenu *menuSelectSymPart;
    static QMenu *menuSelectSheet;

    static QActionPtr cmMenuInsertSymbol;
    static QActionPtr cmMenuInsertSheet;
    static QActionPtr cmMenuInsertPart;
    static QActionPtr cmMenuInsertPart3d;
    static QActionPtr cmMenuInsertPcb;
    static QActionPtr cmMenuInsertComp;
    static QActionPtr cmMenuRules;
    static QActionPtr cmMenuViewPart3d;


    //Tool bars for editor command
    static QToolBar *barMain;
    static QToolBar *barSymbol;
    static QToolBar *barPart;
    static QToolBar *barPart3d;
    static QToolBar *barPart3dView;
    static QToolBar *barComp;
    static QToolBar *barSheet;
    static QToolBar *barPcb;
    static QToolBar *barView;
    static QToolBar *barHelp;

    static void addEditCommands( QToolBar *bar );
    static void addViewCommands( QToolBar *bar );
    static void addDrawCommands( QToolBar *bar );



    static QActionPtr   cmFileNew;
    static QActionPtr   cmFileOpen;
    static QActionPtr   cmFileClose;
    static QActionPtr   cmFileCloseAll;
    static QActionPtr   cmFileSave;
    static QActionPtr   cmFileSaveAs;
    static QActionPtr   cmFileSaveAll;
    static QActionPtr   cmFilePrint;
    static QActionPtr   cmFileExit;
    static QActionPtr   cmFileImportPis;
    static QActionPtr   cmFileImport;
    static QActionPtr   cmFileExport;
    static QActionPtr   cmFileExportGerber;
    static QActionPtr   cmFileStoreToLibrary;
    static QActionPtr   cmFileLoadFromLibrary;
    static QActionPtr   cmFileCloud;

    static QActionPtr   cmFilePrevious[PREVIOUS_FILES_COUNT];

    static QActionPtr   cmObjectNew;
    static QActionPtr   cmObjectLoad;
    static QActionPtr   cmObjectRename;
    static QActionPtr   cmObjectDelete;
    static QActionPtr   cmObjectCopy;
    static QActionPtr   cmObjectPaste;
    static QActionPtr   cmObjectCut;
    static QActionPtr   cmObjectDuplicate;
    static QActionPtr   cmObjectSort;
    static QActionPtr   cmObjectParam;
    static QActionPtr   cmObjectEditEnable;
    static QActionPtr   cmObjectEditDisable;
    static QActionPtr   cmObjectPublic;
    static QActionPtr   cmProjectParam;
    static QActionPtr   cmProjectUpgrade;

    static QActionPtr   cmEditUndo;
    static QActionPtr   cmEditRedo;
    static QActionPtr   cmEditCut;
    static QActionPtr   cmEditCopy;
    static QActionPtr   cmEditCopySvg;
    static QActionPtr   cmEditCopyWeb;
    static QActionPtr   cmEditPaste;
    static QActionPtr   cmEditDelete;
    static QActionPtr   cmEditSelectAll;
    static QActionPtr   cmEditUnSelect;
    static QActionPtr   cmEditSelectSave;
    static QActionPtr   cmEditFind;
    static QActionPtr   cmEditReplace;
    static QActionPtr   cmEditRotateGroup;
    static QActionPtr   cmEditProperties;
    static QActionPtr   cmEditCalculations;
    static QActionPtr   cmEditFragments;

    static QActionPtr   cmContextSelectItem[MCC_SELECT_ITEM_COUNT];
    static QActionPtr   cmContextComponentRotate;
    static QActionPtr   cmContextComponentFlip;
    static QActionPtr   cmContextGroupRotate;
    static QActionPtr   cmContextGraphRotate;
    static QActionPtr   cmContextGraphMirror;


    static QActionPtr   cmViewProject;
    static QActionPtr   cmView3d;
    static QActionPtr   cmViewMirror;
    static QActionPtr   cmViewFill;
    static QActionPtr   cmViewNets;
    static QActionPtr   cmViewGrid;
    static QActionPtr   cmViewLayers;

    static QActionPtr   cm3dMaster;
    static QActionPtr   cm3dProgram;
    static QActionPtr   cm3dImportStl;
    static QActionPtr   cm3dImportStep;
    static QActionPtr   cm3dImportVrml;
    static QActionPtr   cm3dShow2d;
    static QActionPtr   cm3dShowPads;
    static QActionPtr   cm3dTopView;

    static QActionPtr   cmNetSetup;
    static QActionPtr   cmModeLink;
    static QActionPtr   cmModeNetName;
    static QActionPtr   cmModeNetList;
    static QActionPtr   cmModePack;
    static QActionPtr   cmPads;

    static QActionPtr   cmShowRatNet;
    static QActionPtr   cmShowRuleErrors;
    static QActionPtr   cmRenumeration;
    static QActionPtr   cmShowField;
    static QActionPtr   cmShowPads;
    static QActionPtr   cmPcbStratum;

    static QActionPtr   cmRulesEdit;
    static QActionPtr   cmRulesCheck;
    static QActionPtr   cmRulesErrorNext;

    static QActionPtr   cmSymbolPartParam;

    static QActionPtr   cmSheetExpression;

    static QActionPtr   cmOption;
    static QActionPtr   cmTools;

    static QActionPtr   cmHelpContents;
    static QActionPtr   cmHelpIndex;
    static QActionPtr   cmHelpAbout;
    static QActionPtr   cmHelpRegistration;
    static QActionPtr   cmHelpHome;
    static QActionPtr   cmHelpBackward;
    static QActionPtr   cmHelpForward;

    static QActionPtr   cmGuiderDialog;
    static QActionPtr   cmGuiderRecord;
    static QActionPtr   cmGuiderNextStep;

    //Full mode action table
    static QActionPtr   cmModeTable[MD_LAST];

    //Full list mode tool bars
    static SdPropBarPtr mBarTable[PB_LAST];

    //Menu map is to replace menu ident in help page with real root menu title and submenu title
    static QMap<QString,QString> mMenuMap;

    //Set visible properties bar for barId
    static void         activateModeBar(int barId, SdProjectItem *pitem );
    //Get properties bar for barId
    static SdPropBar   *getModeBar( int barId );
    //Get current visible barId
    static int          getModeBarId();

    //!
    //! \brief getModeToolBar Helper function which convert QToolBar returned by getModeBar to derived class
    //! \param barId          Ident of bar
    //! \return               Tool bar reduced to needed type
    //!
    template<typename ToolBar>
    static ToolBar    *getModeToolBar() { return dynamic_cast<ToolBar*>( getModeBar(ToolBar::mBarId) ); }

    static QMenu      *getSelectMenu(SdClass objectClass , SdClass selectedClass);


    static void        createMenu( SdWMain *frame );
    static void        updatePreviousMenu();
    static void        addToPreviousMenu( const QString fname );
    static void        projectState( bool enable );
    static void        createToolBars( SdWMain *frame );
    static void        hideEditorContext();
    static void        selectMode( int md );
    static void        __addMenuMap( const QString &id, QMenu *rootMenu, QAction *menuAction );
  };

//extern SdWCommand sdWCommand;


#define InsertActionAndAssignName( bar, action, name ) bar->insertAction( nullptr, action); if( auto *widget = bar->widgetForAction(action) ) widget->setObjectName( name )

#endif // SDWCOMMAND_H
