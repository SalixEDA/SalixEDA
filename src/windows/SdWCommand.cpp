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

  Functions of this class used to init menus and creation tool bars.

  All command mapping to main window handlers
*/

#include "SdWCommand.h"
#include "SdWMain.h"
#include "SdPropBarLay.h"
#include "SdPropBarDefault.h"
#include "SdPropBarLinear.h"
#include "SdPropBarTextual.h"
#include "SdPropBarSymPin.h"
#include "SdPropBarPartPin.h"
#include "SdPropBarSymImp.h"
#include "SdPropBarPartImp.h"
#include "SdPropBarRoad.h"
#include "SdPropBarPolygon.h"
#include "SdPropBarWire.h"
#include "SdPropBarPartPlace.h"
#include "objects/SdEnvir.h"
#include <QMenuBar>
#include <QSettings>
#include <QFileInfo>
#include <QComboBox>
#include <QDebug>
#include <QShortcut>




void SdWCommand::createMenu(SdWMain *frame)
  {
  //File menu [Меню Файл]
  menuFile = new QMenu( QObject::tr("File") );

  QShortcut *shortcut = new QShortcut(Qt::Key_F8,frame);
  frame->connect( shortcut, &QShortcut::activated, frame, [frame] () {
    QMenuBar *bar = frame->menuBar();
    bar->setFocus();
    bar->setActiveAction(bar->actions().first());
    });

  cmFileNew = menuFile->addAction( QIcon(QString(":/pic/new.png")), QObject::tr("New project"), frame, &SdWMain::cmFileNew );
  cmFileLoadFromLibrary = menuFile->addAction( QIcon(QString(":/pic/download.png")), QObject::tr("Load from library..."), frame, &SdWMain::cmFileLoad );
  cmFileOpen = menuFile->addAction( QIcon(QString(":/pic/fileOpen.png")), QObject::tr("Open project ..."), frame, &SdWMain::cmFileOpen );
  menuFilePrevious = menuFile->addMenu( QObject::tr("Previous files") );
  menuFile->addSeparator();
  cmFileClose = menuFile->addAction( QIcon(QString(":/pic/closeFile.png")), QObject::tr("Close project"), frame, &SdWMain::cmFileClose );
  cmFileCloseAll = menuFile->addAction( QIcon(QString(":/pic/fileCloseAll.png")), QObject::tr("Close all projects"), frame, &SdWMain::cmFileCloseAll );
  menuFile->addSeparator();
  cmFileSave = menuFile->addAction( QIcon(QString(":/pic/save.png")),QObject::tr("Save project"), frame, &SdWMain::cmFileSave );
  cmFileSaveAs = menuFile->addAction( QIcon(QString(":/pic/save_as.png")), QObject::tr("Save project as..."), frame, &SdWMain::cmFileSaveAs );
  cmFileSaveAll = menuFile->addAction( QIcon(QString(":/pic/save_all.png")), QObject::tr("Save all projects"), frame, &SdWMain::cmFileSaveAll );
  cmFileStoreToLibrary = menuFile->addAction( QIcon(QString(":/pic/upload.png")),QObject::tr("Store to library..."), frame, &SdWMain::cmFileStore );
  cmFileCloud = menuFile->addAction( QIcon(QStringLiteral(":/pic/projectCloud.png")), QObject::tr("Project stored in library"), frame, &SdWMain::cmFileCloud );
  cmFileCloud->setCheckable(true);
  menuFile->addSeparator();
  cmFileImportPis = menuFile->addAction( QObject::tr("Import PasCAD"), frame, &SdWMain::cmFileImportPis );
  cmFileImport    = menuFile->addAction( QIcon(QString(":/pic/fileImport.png")), QObject::tr("Import..."), frame, &SdWMain::cmFileImport );
  cmFileExport    = menuFile->addAction( QIcon(QString(":/pic/fileExport.png")), QObject::tr("Export..."), frame, &SdWMain::cmFileExport );
  menuFile->addSeparator();
  cmFilePrint = menuFile->addAction( QIcon(QString(":/pic/filePrint.png")), QObject::tr("Print..."), frame, &SdWMain::cmDelegate<&SdWEditor::cmFilePrint> );
  menuFile->addSeparator();
  cmFileExit = menuFile->addAction( QIcon(QString(":/pic/exit.png")), QObject::tr("Exit programm"), frame, &SdWMain::cmFileExit );

  //Last previous files menu [Меню для предыдущих файлов]
  for (int i = 0; i < PREVIOUS_FILES_COUNT; ++i)
    {
    cmFilePrevious[i] = new QAction(frame);
    cmFilePrevious[i]->setVisible(false);
    frame->connect(cmFilePrevious[i], &QAction::triggered, frame, &SdWMain::cmFilePrevious );
    menuFilePrevious->addAction(cmFilePrevious[i]);
    }

  updatePreviousMenu();


  //Object menu [Меню Объект]
  menuObject = new QMenu( QObject::tr("Objects") );
  cmObjectNew         = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectNew.png")), QObject::tr("Create..."), frame, &SdWMain::cmObjectNew );
  cmObjectLoad        = menuObject->addAction( QIcon(QStringLiteral(":/pic/download.png")), QObject::tr("Load from library..."), frame, &SdWMain::cmObjectLoad );
  cmObjectRename      = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectRename.png")), QObject::tr("Rename..."), frame, &SdWMain::cmObjectRename );
  cmObjectParam       = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectParam.png")), QObject::tr("Edit object param..."), frame, &SdWMain::cmObjectParam );
  cmObjectDelete      = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectDelete.png")), QObject::tr("Delete"), frame, &SdWMain::cmObjectDelete );
  cmObjectCopy        = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectCopy.png")), QObject::tr("Copy"), frame, &SdWMain::cmObjectCopy );
  cmObjectPaste       = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectPaste.png")), QObject::tr("Paste"), frame, &SdWMain::cmObjectPaste );
  cmObjectCut         = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectCut.png")), QObject::tr("Cut"), frame, &SdWMain::cmObjectCut );
  cmObjectDuplicate   = menuObject->addAction( QIcon(QStringLiteral(":/pic/duplicate.png")), QObject::tr("Duplicate"), frame, &SdWMain::cmObjectDuplicate );
  cmObjectSort        = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectSort.png")), QObject::tr("Sort"), frame, &SdWMain::cmObjectSort );
  cmObjectEditEnable  = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectEditEnable.png")), QObject::tr("Enable edit"), frame, &SdWMain::cmObjectEditEnable );
  cmObjectEditDisable = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectEditDisable.png")), QObject::tr("Finish edit"), frame, &SdWMain::cmObjectEditDisable );
  cmObjectPublic      = menuObject->addAction( QIcon(QStringLiteral(":/pic/objectPublic.png")), QObject::tr("Make an object public"), frame, &SdWMain::cmObjectPublic );
  cmObjectPublic->setCheckable(true);
  cmProjectParam      = menuObject->addAction( QIcon(QStringLiteral(":/pic/editProjectParam.png")), QObject::tr("Edit project param..."), frame, &SdWMain::cmProjectParam );
  cmProjectUpgrade    = menuObject->addAction( QIcon(QStringLiteral(":/pic/editProjectUpgrade.png")), QObject::tr("Upgrade with new versions"), frame, &SdWMain::cmProjectUpgrade );




  //Edit menu [Меню Редактирование]
  menuEdit = new QMenu( QObject::tr("Edit") );
  cmEditUndo      = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editUndo.png")), QObject::tr("Undo"), frame, &SdWMain::cmEditUndo );
  cmEditUndo->setEnabled(false);
  cmEditRedo      = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editRedo.png")), QObject::tr("Redo"), frame, &SdWMain::cmEditRedo );
  cmEditRedo->setEnabled(false);
  menuEdit->addSeparator();
  cmEditCopy      = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCopy.png")), QObject::tr("Copy"), frame, &SdWMain::cmEditCopy );
  cmEditPaste     = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editPaste.png")), QObject::tr("Paste"), frame, &SdWMain::cmEditPaste );
  cmEditCut       = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCut.png")), QObject::tr("Cut"), frame, &SdWMain::cmEditCut );
  cmEditDelete    = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editDel.png")), QObject::tr("Delete"), frame, &SdWMain::cmEditDelete );
  menuEdit->addSeparator();
  cmEditCopySvg   = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCopySvg.png")), QObject::tr("Copy SVG"), frame, &SdWMain::cmEditCopySvg );
  cmEditCopyWeb   = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCopyWeb.png")), QObject::tr("Copy WEB"), frame, &SdWMain::cmEditCopyWeb );
  cmEditSelectSave= menuEdit->addAction( QIcon(QStringLiteral(":/pic/editSelectSave.png")), QObject::tr("Save selection"), frame, &SdWMain::cmEditSelectSave );
  menuEdit->addSeparator();
  cmEditSelectAll = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editSelectAll.png")), QObject::tr("Select All"), frame, &SdWMain::cmEditSelectAll );
  cmEditUnSelect  = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editUnselect.png")), QObject::tr("UnSelect"), frame, &SdWMain::cmEditUnSelect );
  cmEditFind      = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editFind.png")), QObject::tr("Find"), frame, &SdWMain::cmEditFind );
  cmEditReplace   = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editReplace.png")), QObject::tr("Replace"), frame, &SdWMain::cmEditReplace );
  cmEditRotateGroup = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editRotate.png")), QObject::tr("Rotate group 90 degree [F7]"), frame, &SdWMain::cmEditRotateGroup );
  menuEdit->addSeparator();
  cmEditProperties  = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editProp.png")), QObject::tr("Params of selection..."), frame, &SdWMain::cmEditProperties );
  cmEditCalculations = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editCalculations.png")), QObject::tr("Calculations..."), frame, &SdWMain::cmEditCalculations );
  cmEditFragments   = menuEdit->addAction( QIcon(QStringLiteral(":/pic/editFragments.png")), QObject::tr("Fragments..."), frame, &SdWMain::cmEditFragments );



  //View menu
  menuView = new QMenu( QObject::tr("View") );
  cmViewProject = menuView->addAction( QIcon(QString(":/pic/openProjects.png")), QObject::tr("Show-hide project"), frame, &SdWMain::cmViewProject );
  menuView->addSeparator();
  cmView3d   = menuView->addAction( QIcon(QStringLiteral(":/pic/view3d.png")), QObject::tr("3d-2d switch"), frame, &SdWMain::cmView3d );
  cmView3d->setEnabled(false);
  cmViewMirror = menuView->addAction( QObject::tr("Mirror"), frame, &SdWMain::cmViewMirrorHorz );
  cmViewMirror->setCheckable(true);
  cmViewMirror->setEnabled(false);
  cmViewNets = menuView->addAction( QIcon(QString(":/pic/viewRatnet.png")), QObject::tr("Nets"), frame, &SdWMain::cmViewNets );
  cmViewGrid = menuView->addAction( QIcon(QString(":/pic/viewGrid.png")), QObject::tr("Grid"), frame, &SdWMain::cmViewGrid );
  cmViewGrid->setToolTip( QObject::tr("Show grid tune dialog") );
  cmViewLayers = menuView->addAction( QIcon(QString(":/pic/layers.png")), QObject::tr("Layers"), frame, &SdWMain::cmViewLayers );
  cmViewLayers->setToolTip( QObject::tr("Show layers editor dialog") );
  menuView->addSeparator();
  cmViewFill = menuView->addAction( QIcon(QString(":/pic/viewFit.png")), QObject::tr("Zoom to fit view"), frame, &SdWMain::cmViewFill );
  cmModeTable[MD_ZOOM_IN]     = menuView->addAction( QIcon(QString(":/pic/viewZoomIn.png")), QObject::tr("Zoom in"), frame, &SdWMain::cmViewZoomIn );
  cmModeTable[MD_ZOOM_OUT]    = menuView->addAction( QIcon(QString(":/pic/viewZoomOut.png")), QObject::tr("Zoom out"), frame, &SdWMain::cmViewZoomOut );
  cmModeTable[MD_ZOOM_WIN]   = menuView->addAction( QIcon(QString(":/pic/viewWin.png")), QObject::tr("Zoom area"), frame, &SdWMain::cmViewArea );
  menuView->addSeparator();
  cmModeTable[MD_MEASUREMENT] = menuView->addAction( QIcon(QString(":/pic/viewRuller.png")), QObject::tr("Measurement"), frame, &SdWMain::cmViewMeasurement );



  //Draw menu
  menuDraw = new QMenu( QObject::tr("Draw") );
  cmModeTable[MD_SELECT]      = menuDraw->addAction( QIcon(QString(":/pic/select.png")), QObject::tr("Select and edit"), frame, &SdWMain::cmModeSelect );
  cmModeTable[MD_BINDER]      = menuDraw->addAction( QIcon(QString(":/pic/objPrtPin.png")), QObject::tr("Binder"), frame, &SdWMain::cmModeBinder );
  menuDraw->addSeparator();
  cmModeTable[MD_LINE]        = menuDraw->addAction( QIcon(QString(":/pic/drawLine.png")), QObject::tr("Line"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeLine> );
  cmModeTable[MD_RECT]        = menuDraw->addAction( QIcon(QString(":/pic/drawRect.png")), QObject::tr("Rect"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeRect> );
  cmModeTable[MD_FILL_RECT]   = menuDraw->addAction( QIcon(QString(":/pic/drawFRect.png")), QObject::tr("Filled rect"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeFilledRect> );
  cmModeTable[MD_REGION]      = menuDraw->addAction( QIcon(QString(":/pic/drawRegion.png")), QObject::tr("Region"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeRegion> );
  cmModeTable[MD_FILL_REGION] = menuDraw->addAction( QIcon(QString(":/pic/drawFRegion.png")), QObject::tr("Filled region"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeFilledRegion> );
  cmModeTable[MD_CIRCLE]      = menuDraw->addAction( QIcon(QString(":/pic/drawCircle.png")), QObject::tr("Circle"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeCircle> );
  cmModeTable[MD_FILL_CIRCLE] = menuDraw->addAction( QIcon(QString(":/pic/drawFCircle.png")), QObject::tr("Filled circle"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeFilledCircle> );
  cmModeTable[MD_ARC]         = menuDraw->addAction( QIcon(QString(":/pic/drawArc.png")), QObject::tr("Arc"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeArc> );
  cmModeTable[MD_TEXT]        = menuDraw->addAction( QIcon(QString(":/pic/drawText.png")), QObject::tr("Text"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeText> );
  cmShowField = menuDraw->addAction( QIcon(QStringLiteral(":/pic/showField.png")), QObject::tr("Show fields or values") );
  cmShowField->setCheckable(true);
  cmShowField->setChecked( SdEnvir::instance()->mShowFields );
  cmShowField->connect( cmShowField, &QAction::toggled, frame, &SdWMain::cmShowFields );



  //Symbol editor menu
  menuInsertSymbol = new QMenu( QObject::tr("Symbol") );

  cmModeTable[MD_SYM_PIN]         = menuInsertSymbol->addAction( QIcon(QString(":/pic/objPin.png")), QObject::tr("Insert pin"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModePin> );
  cmModeTable[MD_SYM_IDENT]       = menuInsertSymbol->addAction( QIcon(QString(":/pic/objIdent.png")), QObject::tr("Edit reference"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeReference> );
  cmModeTable[MD_SYM_ORIGIN]      = menuInsertSymbol->addAction( QIcon(QString(":/pic/objOrigin.png")), QObject::tr("Edit origin"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeOrigin> );
  cmModeTable[MD_SYM_VALUE]       = menuInsertSymbol->addAction( QIcon(QString(":/pic/objValue.png")), QObject::tr("Edit value"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeValue> );
  cmModeTable[MD_SYMBOL_FRAGMENT] = menuInsertSymbol->addAction( QIcon(QString(":/pic/symbolFragment.png")), QObject::tr("Enter symbol fragment"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeSymbolFragment> );
  menuInsertSymbol->addSeparator();
  cmSymbolPartParam               = menuInsertSymbol->addAction( QIcon(QString(":/pic/objSymbolPartParam.png")), QObject::tr("Default part and params"), frame, &SdWMain::cmDelegate<&SdWEditor::cmSymbolPartParam> );



  //Part editor menu
  menuInsertPart = new QMenu( QObject::tr("Part") );

  cmShowPads                     = menuInsertPart->addAction( QIcon(QString(":/pic/objVia.png")), QObject::tr("Show pads") );
  cmShowPads->setCheckable(true);
  cmShowPads->setChecked( SdEnvir::instance()->mShowPads );
  cmShowPads->connect( cmShowPads, &QAction::toggled, frame, &SdWMain::cmShowPads );
  cmModeTable[MD_PART_PIN]       = menuInsertPart->addAction( QIcon(QString(":/pic/objPrtPin.png")), QObject::tr("Insert pin"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModePin> );
  cmModeTable[MD_PART_IDENT]     = menuInsertPart->addAction( QIcon(QString(":/pic/objIdent.png")), QObject::tr("Edit reference"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeReference> );
  cmModeTable[MD_PART_ORIGIN]    = menuInsertPart->addAction( QIcon(QString(":/pic/objOrigin.png")), QObject::tr("Edit origin"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeOrigin> );
  cmModeTable[MD_PART_VALUE]     = menuInsertPart->addAction( QIcon(QString(":/pic/objPrtValue.png")), QObject::tr("Edit value"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeValue> );



  //3D part editor menu
  menuInsertPart3d = new QMenu( QObject::tr("3d part") );
  cm3dMaster           = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dmaster.png")), QObject::tr("Master of 3d form"), frame, &SdWMain::cm3dMaster );
  cm3dProgram          = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dProgram.png")), QObject::tr("Edit 3d model script"), frame, &SdWMain::cm3dProgram );
  cm3dImportStl        = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dImportStl.png")), QObject::tr("Import from stl file"), frame, &SdWMain::cm3dImportStl );
  //cm3dImportStep       = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dImportStep.png")), QObject::tr("Import from step file"), frame, SLOT(cm3dImportStep()) );
  cm3dImportVrml       = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dImportVrml.png")), QObject::tr("Import from vrml file"), frame, &SdWMain::cm3dImportVrml );
  menuInsertPart3d->addSeparator();
  cm3dShow2d                     = menuInsertPart3d->addAction( QIcon(QString(":/pic/drawRect.png")), QObject::tr("Show/hide 2d graphics in 3d scene"), frame, &SdWMain::cm3dShow2d );
  cm3dShow2d->setCheckable(true);
  cm3dShowPads                   = menuInsertPart3d->addAction( QIcon(QString(":/pic/objVia.png")), QObject::tr("Show/hide pads"), frame, &SdWMain::cm3dShowPads );
  cm3dShowPads->setCheckable(true);
  cm3dTopView                    = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dTopView.png")), QObject::tr("Set top view orientation"), frame, &SdWMain::cmDelegate<&SdWEditor::cm3dTopView> );
  menuInsertPart3d->addSeparator();
  cmModeTable[MD_3D_VIEW]        = menuInsertPart3d->addAction( QIcon(QString(":/pic/select.png")), QObject::tr("View 3d scene with no edit"), frame, &SdWMain::cm3dModeView );
  cmModeTable[MD_3D_HORZ_MOVE]   = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dHorzMove.png")), QObject::tr("Horizontal moving 3d object"), frame, &SdWMain::cm3dModeHorzMove );
  cmModeTable[MD_3D_HORZ_ROTATE] = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dHorzRotate.png")), QObject::tr("Horizontal rotation 3d object"), frame, &SdWMain::cm3dModeHorzRotate );
  cmModeTable[MD_3D_VERT_MOVE]   = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dVertMove.png")), QObject::tr("Vertical moving 3d object"), frame, &SdWMain::cm3dModeVertMove );
  cmModeTable[MD_3D_VERT_ROTATE] = menuInsertPart3d->addAction( QIcon(QString(":/pic/3dVertRotate.png")), QObject::tr("Vertical rotation 3d object"), frame, &SdWMain::cm3dModeVertRotate );
  cmModeTable[MD_3D_FACE_COLOR]  = menuInsertPart3d->addAction( QIcon(QString(":/pic/colors.png")), QObject::tr("Change face color"), frame, &SdWMain::cm3dModeFaceColor );



  //3D part view menu
  menuViewPart3d = new QMenu( QObject::tr("3d part view") );
  menuViewPart3d->addAction( cm3dProgram );
  menuViewPart3d->addSeparator();
  menuViewPart3d->addAction( cm3dShow2d );
  menuViewPart3d->addAction( cm3dShowPads );
  menuViewPart3d->addAction( cm3dTopView );


  menuInsertComp = new QMenu( QObject::tr("Insert comp") );



  //Sheet editor menu
  menuInsertSheet = new QMenu( QObject::tr("Sheet") );
  cmRenumeration              = menuInsertSheet->addAction( QIcon(QStringLiteral(":/pic/renumeration.png")), QObject::tr("Renumeration"), frame, &SdWMain::cmRenumeration );
  __addMenuMap( "menu:sheetRenumeration", menuInsertSheet, cmRenumeration );
  auto formMaster             = menuInsertSheet->addAction( QIcon(QStringLiteral(":/pic/formMaster.png")), QObject::tr("Sheet form master..."), frame, &SdWMain::cmDelegate<&SdWEditor::cmFormInsert> );
  __addMenuMap( "menu:formMaster", menuInsertSheet, formMaster );
  menuInsertSheet->addSeparator();
  cmModeTable[MD_FRAGMENT]    = menuInsertSheet->addAction( QIcon(QString(":/pic/iconSheet.png")), QObject::tr("Insert fragment"), frame, &SdWMain::cmModeFragment );
  cmModeTable[MD_COMPONENT]   = menuInsertSheet->addAction( QIcon(QString(":/pic/objComp.png")), QObject::tr("Insert component"), frame, &SdWMain::cmModeComponent );
  cmModeTable[MD_NET]         = menuInsertSheet->addAction( QIcon(QString(":/pic/objWire.png")), QObject::tr("Insert net wire"), frame, &SdWMain::cmModeNet );
  cmModeTable[MD_NET_NAME]    = menuInsertSheet->addAction( QIcon(QString(":/pic/objWireName.png")), QObject::tr("Insert net name"), frame, &SdWMain::cmModeNetName );
  cmModeTable[MD_BUS]         = menuInsertSheet->addAction( QIcon(QString(":/pic/objBus.png")), QObject::tr("Insert bus (many net)"), frame, &SdWMain::cmModeBus );
  cmModeTable[MD_DISCONNECT]  = menuInsertSheet->addAction( QIcon(QString(":/pic/objUnConnect.png")), QObject::tr("Disconnect net"), frame, &SdWMain::cmModeDisconnect );
  cmModeTable[MD_SHEET_IDENT] = menuInsertSheet->addAction( QIcon(QString(":/pic/objIdent.png")), QObject::tr("Move ident of components"), frame, &SdWMain::cmModeReferenceMove );
  cmModeTable[MD_SHEET_VALUE] = menuInsertSheet->addAction( QIcon(QString(":/pic/objValue.png")), QObject::tr("Move value of components"), frame, &SdWMain::cmModeValueMove );
  cmModeTable[MD_SCRIPT]      = menuInsertSheet->addAction( QIcon(QString(":/pic/objScript.png")), QObject::tr("Script insert and edit"), frame, &SdWMain::cmModeScript );
  cmModeTable[MD_NET_LIST]    = menuInsertSheet->addAction( QIcon(QString(":/pic/netPinList.png")), QObject::tr("Net pins list insert and edit"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeNetList> );
  //cmModeTable[MD_PCB_AREA]   = menuInsertSheet->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
//  cmModeTable[MD_FIELD]      = menuInsertSheet->addAction( QIcon(QString(":/pic/objField.png")), QObject::tr("Field"), frame, SLOT(cmModeF) );



  //PCB editor menu
  menuInsertPcb = new QMenu( QObject::tr("Plate") );
  menuInsertPcb->insertAction( nullptr, cmRenumeration );
  __addMenuMap( "menu:plateRenumeration", menuInsertPcb, cmRenumeration );
  cmPcbStratum  = menuInsertPcb->addAction( QIcon(QString(":/pic/iconViaThrow.png")), QObject::tr("PCB layer count..."), frame, &SdWMain::cmPcbStratum );
  auto boardMaster = menuInsertPcb->addAction( QIcon(QString(":/pic/boardMaster.png")), QObject::tr("PCB board builder..."), frame, &SdWMain::cmDelegate<&SdWEditor::cmBoardBuilder> );
  __addMenuMap( "menu:plateBoardMaster", menuInsertPcb, boardMaster );
  menuInsertPcb->insertSeparator(nullptr);
  cmShowRatNet = menuInsertPcb->addAction( QIcon(QStringLiteral(":/pic/viewRatnet.png")), QObject::tr("Show rat net") );
  cmShowRatNet->setCheckable(true);
  cmShowRatNet->setChecked( SdEnvir::instance()->mShowRatNet );
  cmShowRatNet->connect( cmShowRatNet, &QAction::toggled, frame, &SdWMain::cmShowRatNet );
  cmPads = menuInsertPcb->addAction( QIcon(QStringLiteral(":/pic/pads.png")), QObject::tr("Pads association"), frame, &SdWMain::cmPads );
  //cmShowRatNet->co
//  cmNetSetup       = menuInsertPcb->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
//  cmPads           = menuInsertPcb->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
//  menuInsertPcb->addSeparator();
  //  cmModeLink       = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeNetName    = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeNetList    = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  // cmModePack       = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeLineSize   = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeRadiusSize = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  cmModeTable[MD_PART_IMP]   = menuInsertPcb->addAction( QIcon(QString(":/pic/objPrt.png")), QObject::tr("Insert part"), frame, &SdWMain::cmModeComponent );
  cmModeTable[MD_MOVE_PART]  = menuInsertPcb->addAction( QIcon(QString(":/pic/objPrtPlace.png")), QObject::tr("Move part"), frame, &SdWMain::cmModeMovePart );
  cmModeTable[MD_ROAD_ENTER] = menuInsertPcb->addAction( QIcon(QString(":/pic/objPcbWire.png")), QObject::tr("Road enter"), frame, &SdWMain::cmModeRoadEnter );
  cmModeTable[MD_ROAD_MOVE]  = menuInsertPcb->addAction( QIcon(QString(":/pic/objPcbWireMove.png")), QObject::tr("Road move"), frame, &SdWMain::cmModeRoadMove );
  cmModeTable[MD_POLYGON]    = menuInsertPcb->addAction( QIcon(QString(":/pic/objPolygon.png")), QObject::tr("Polygon enter"), frame, &SdWMain::cmModePolygon );
  cmModeTable[MD_VIA_ENTER]  = menuInsertPcb->addAction( QIcon(QString(":/pic/objVia.png")), QObject::tr("Via enter"), frame, &SdWMain::cmModeViaEnter );
  cmModeTable[MD_TRACE_DELETE]   = menuInsertPcb->addAction( QIcon(QString(":/pic/objPcbWireDel.png")), QObject::tr("Delete trace"), frame, &SdWMain::cmModeTraceDelete );
  //  cmModePlace      = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeEditWire   = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeWire       = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModeDeleteWire = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  //  cmModePad        = menuInsert->addAction( QIcon(QString(":/pic/.png")), QObject::tr(""), frame, SLO );
  cmModeTable[MD_PLATE_IDENT] = menuInsertPcb->addAction( QIcon(QString(":/pic/objIdent.png")), QObject::tr("Move ident of components"), frame, &SdWMain::cmModeReferenceMove );
  cmModeTable[MD_PLATE_VALUE] = menuInsertPcb->addAction( QIcon(QString(":/pic/objPrtValue.png")), QObject::tr("Move value of components"), frame, &SdWMain::cmModeValueMove );
  cmModeTable[MD_PLATE_ORIGIN]= menuInsertPcb->addAction( QIcon(QString(":/pic/objPlateOrigin.png")), QObject::tr("Edit plate origin"), frame, &SdWMain::cmDelegate<&SdWEditor::cmModeOrigin> );
  cmModeTable[MD_PART_HIGHLIGHT] = menuInsertPcb->addAction( QIcon(QString(":/pic/objPartHighlight.png")), QObject::tr("Highlight components"), frame, &SdWMain::cmModePartHighlight );


  //PCB roads rules menu
  menuRules = new QMenu( QObject::tr("Rules") );
  cmRulesEdit = menuRules->addAction( QIcon(QStringLiteral(":/pic/rules.png")), QObject::tr("Edit rules dialog"), frame, &SdWMain::cmRulesEdit );
  cmShowRuleErrors = menuRules->addAction( QIcon(QStringLiteral(":/pic/objShowRuleErrors.png")), QObject::tr("Show rule errors") );
  cmShowRuleErrors->setCheckable(true);
  cmShowRuleErrors->setChecked( SdEnvir::instance()->mShowRuleErrors );
  cmShowRuleErrors->connect( cmShowRuleErrors, &QAction::toggled, frame, &SdWMain::cmShowRuleErrors );
  cmRulesCheck = menuRules->addAction( QIcon(QStringLiteral(":/pic/objRulesCheck.png")), QObject::tr("Check all rules"), frame, &SdWMain::cmRulesCheck );
  cmRulesErrorNext = menuRules->addAction( QIcon(QStringLiteral(":/pic/rulesNextError.png")), QObject::tr("Center next rules error"), frame, &SdWMain::cmRulesErrorNext );


  //Instruments menu
  menuInstruments = new QMenu( QObject::tr("Instruments") );
  //menuInstruments->addAction( QIcon(QString(":/pic/library.png")), QObject::tr("Library"), frame, &SdWMain::cmLibrary );
  cmOption = menuInstruments->addAction( QIcon(QString(":/pic/instrumOptions.png")), QObject::tr("Options"), frame, &SdWMain::cmOption );
  menuInstruments->addSeparator();
  cmTools  = menuInstruments->addAction( QIcon(QString(":/pic/instrumTools.png")), QObject::tr("Ref to library object"), frame, &SdWMain::cmRef2LibraryObject );
             menuInstruments->addAction( QObject::tr("Library service"), frame, &SdWMain::cmLibrary );
  menuInstruments->addSeparator();
  cmGuiderDialog     = menuInstruments->addAction( QObject::tr("Guider scene builder..."), frame, &SdWMain::cmGuiderDialog );
                       menuInstruments->addAction( QObject::tr("Guider snapshot save"), QKeySequence(Qt::Key_F12), frame, &SdWMain::cmGuiderSnapshotSave );
//  cmGuiderRecord     = menuInstruments->addAction( QObject::tr("Guifrt record start-stop"), QKeySequence(Qt::Key_F11), frame, &SdWMain::cmGuiderRecord );
//  cmGuiderRecord->setCheckable(true);
//  cmGuiderNextStep   = menuInstruments->addAction( QObject::tr("Guider record next step"), QKeySequence(Qt::Key_F10), frame, &SdWMain::cmGuiderNextStep );
                       // menuInstruments->addAction( QObject::tr("Guider play scene"), frame, &SdWMain::cmGuiderPlay );
                       // menuInstruments->addAction( QObject::tr("Guider capture scene"), frame, &SdWMain::cmGuiderCapture );



  //Help menu
  menuHelp = new QMenu( QObject::tr("Help") );
                       menuHelp->addAction( QIcon(QString(":/pic/aiChat.png")), QObject::tr("Ai Chat"), frame, &SdWMain::cmHelpAi );
  cmHelpContents     = menuHelp->addAction( QIcon(QString(":/pic/help.png")), QObject::tr("Contents"), frame, &SdWMain::cmHelpContents );
  cmHelpIndex        = menuHelp->addAction( QIcon(QString(":/pic/helpContext.png")), QObject::tr("Index"), frame, &SdWMain::cmHelpIndex );
  cmHelpAbout        = menuHelp->addAction( QIcon(QString(":/pic/helpAbout.png")), QObject::tr("About"), frame, &SdWMain::cmHelpAbout );
  cmHelpRegistration = menuHelp->addAction( QIcon(QString(":/pic/helpRegistration.png")), QObject::tr("Registration"), frame, &SdWMain::cmHelpRegistration );
  cmHelpHome         = new QAction( QIcon(QString(":/pic/helpHome.png")), QObject::tr("Home help page") );
  frame->connect( cmHelpHome, &QAction::triggered, frame, &SdWMain::cmHelpContents );
  cmHelpBackward     = new QAction( QIcon(QString(":/pic/helpPrevious.png")), QObject::tr("Backward help page") );
  frame->connect( cmHelpBackward, &QAction::triggered, frame, &SdWMain::cmHelpBackward );
  cmHelpForward      = new QAction( QIcon(QString(":/pic/helpNext.png")), QObject::tr("Forward help page") );
  frame->connect( cmHelpForward, &QAction::triggered, frame, &SdWMain::cmHelpForward );


  //Context menu
  QMenu *contextMenu = new QMenu( QObject::tr("Context") );
  for( int i = 0; i <= MCC_SELECT_ITEM_COUNT; ++i )
    cmContextSelectItem[i] = contextMenu->addAction( QObject::tr("Select"), [frame,i]() { frame->cmContextCommand(i + MCC_SELECT_ITEM_FIRST); } );
  cmContextComponentRotate = contextMenu->addAction( QIcon(QString(":/pic/aiChat.png")), QObject::tr("Rotate component CW 90"), frame, &SdWMain::cmContextCommand<MCC_COMPONENT_ROTATE> );
  cmContextComponentFlip   = contextMenu->addAction( QIcon(QString(":/pic/aiChat.png")), QObject::tr("Flip component to other side"), frame, &SdWMain::cmContextCommand<MCC_COMPONENT_FLIP> );
  cmContextGroupRotate     = contextMenu->addAction( QIcon(QString(":/pic/aiChat.png")), QObject::tr("Rotate component CW 90"), frame, &SdWMain::cmContextCommand<MCC_GROUP_ROTATE> );



  //Select popup menu (common version)
  menuSelect = new QMenu( QObject::tr("Select") );
  menuSelect->insertAction( nullptr, cmEditUndo );
  menuSelect->insertAction( nullptr, cmEditRedo );
  menuSelect->addSeparator();
  menuSelect->insertAction( nullptr, cmEditCopy );
  menuSelect->insertAction( nullptr, cmEditPaste );
  menuSelect->insertAction( nullptr, cmEditCut );
  menuSelect->insertAction( nullptr, cmEditDelete );
  menuSelect->addSeparator();
  menuSelect->insertAction( nullptr, cmEditSelectAll );
  menuSelect->insertAction( nullptr, cmEditUnSelect );
  menuSelect->insertAction( nullptr, cmEditFind );
  menuSelect->insertAction( nullptr, cmEditReplace );
  menuSelect->insertAction( nullptr, cmEditRotateGroup );
  menuSelect->addSeparator();
  menuSelect->insertAction( nullptr, cmEditProperties );

  //Select popup menu (symbol and part version)
  menuSelectSymPart = new QMenu( QObject::tr("Select") );
  menuSelectSymPart->insertAction( nullptr, cmEditUndo );
  menuSelectSymPart->insertAction( nullptr, cmEditRedo );
  menuSelectSymPart->addSeparator();
  menuSelectSymPart->insertAction( nullptr, cmEditCopy );
  menuSelectSymPart->insertAction( nullptr, cmEditPaste );
  menuSelectSymPart->insertAction( nullptr, cmEditCut );
  menuSelectSymPart->insertAction( nullptr, cmEditDelete );
  menuSelectSymPart->addSeparator();
  menuSelectSymPart->insertAction( nullptr, cmEditSelectAll );
  menuSelectSymPart->insertAction( nullptr, cmEditUnSelect );
  menuSelectSymPart->insertAction( nullptr, cmEditFind );
  menuSelectSymPart->insertAction( nullptr, cmEditReplace );
  menuSelectSymPart->insertAction( nullptr, cmEditRotateGroup );
  menuSelectSymPart->addSeparator();
  menuSelectSymPart->insertAction( nullptr, cmEditProperties );

  //Select popup menu (sheet version)
  menuSelectSheet = new QMenu( QObject::tr("Select") );
  menuSelectSheet->insertAction( nullptr, cmEditUndo );
  menuSelectSheet->insertAction( nullptr, cmEditRedo );
  menuSelectSheet->addSeparator();
  menuSelectSheet->insertAction( nullptr, cmEditCopy );
  menuSelectSheet->insertAction( nullptr, cmEditPaste );
  menuSelectSheet->insertAction( nullptr, cmEditCut );
  menuSelectSheet->insertAction( nullptr, cmEditDelete );
  menuSelectSheet->addSeparator();
  menuSelectSheet->insertAction( nullptr, cmEditSelectAll );
  menuSelectSheet->insertAction( nullptr, cmEditUnSelect );
  menuSelectSheet->insertAction( nullptr, cmEditFind );
  menuSelectSheet->insertAction( nullptr, cmEditReplace );
  menuSelectSheet->insertAction( nullptr, cmEditRotateGroup );
  menuSelectSheet->addSeparator();
  menuSelectSheet->insertAction( nullptr, cmEditProperties );
  menuSelectSheet->insertAction( nullptr, cmEditCalculations );
  menuSelectSheet->insertAction( nullptr, cmEditFragments );



  QMenuBar *bar = frame->menuBar();
  bar->addMenu( menuFile );
  bar->addMenu( menuObject );
  bar->addMenu( menuEdit );
  bar->addMenu( menuView );
  cmMenuViewPart3d   = bar->addMenu( menuViewPart3d );
  bar->addMenu( menuDraw );
  cmMenuInsertSymbol = bar->addMenu( menuInsertSymbol );
  cmMenuInsertPart   = bar->addMenu( menuInsertPart );
  cmMenuInsertPart3d = bar->addMenu( menuInsertPart3d );
  cmMenuInsertComp   = bar->addMenu( menuInsertComp );
  cmMenuInsertSheet  = bar->addMenu( menuInsertSheet );
  cmMenuInsertPcb    = bar->addMenu( menuInsertPcb );
  cmMenuRules        = bar->addMenu( menuRules );
  bar->addMenu( menuInstruments );
  bar->addMenu( menuHelp );

  projectState(false);
  }





void SdWCommand::updatePreviousMenu()
  {
  //Заполнить меню предыдущими файлами
  QSettings settings;
  QStringList files = settings.value(SDK_PREVIOUS_FILES).toStringList();

  int numRecentFiles = qMin(files.size(), static_cast<int>(PREVIOUS_FILES_COUNT) );

  for (int i = 0; i < numRecentFiles; ++i)
    {
    QString text = QString("&%1 %2").arg(i + 1).arg( QFileInfo(files[i]).filePath() );
    cmFilePrevious[i]->setText(text);
    cmFilePrevious[i]->setData(files[i]);
    cmFilePrevious[i]->setVisible(true);
    }
  for (int j = numRecentFiles; j < PREVIOUS_FILES_COUNT; ++j)
    cmFilePrevious[j]->setVisible(false);

  //Сделать доступным menuFilePrevious
  menuFilePrevious->setEnabled( numRecentFiles != 0 );
  }




void SdWCommand::addToPreviousMenu(const QString fname)
  {
  if( !fname.isEmpty() && fname != SD_DEFAULT_FILE_NAME ) {
    //Если файл имеет неверное расширение, то не открываем
    QSettings settings;
    QStringList files = settings.value(SDK_PREVIOUS_FILES).toStringList();
    files.removeAll(fname);
    files.prepend(fname);
    while( files.size() > PREVIOUS_FILES_COUNT )
      files.removeLast();

    settings.setValue(SDK_PREVIOUS_FILES, files);

    //Обновить меню
    updatePreviousMenu();
    }
  }




void SdWCommand::projectState(bool enable)
  {
  if( !enable ) {
    cmFileSave->setEnabled(enable);
    cmFilePrint->setEnabled(enable);
    cmFileImport->setEnabled(enable);
    cmFileExport->setEnabled(enable);

    cmObjectRename->setEnabled(enable);
    cmObjectParam->setEnabled(enable);
    cmObjectDelete->setEnabled(enable);
    cmObjectCopy->setEnabled(enable);
    cmObjectCut->setEnabled(enable);
    cmObjectDuplicate->setEnabled(enable);

    cmObjectPaste->setEnabled(enable);
    }
  cmFileClose->setEnabled(enable);
  cmFileCloseAll->setEnabled(enable);
  cmFileSaveAs->setEnabled(enable);
  cmFileSaveAll->setEnabled(enable);

  cmObjectNew->setEnabled(enable);
  cmObjectLoad->setEnabled(enable);
  cmObjectSort->setEnabled(enable);
  }




void SdWCommand::addEditCommands(QToolBar *bar)
  {
  InsertActionAndAssignName( bar, cmEditCopy, "cmEditCopy" );
  InsertActionAndAssignName( bar, cmEditCut, "cmEditCut" );
  InsertActionAndAssignName( bar, cmEditPaste, "cmEditPaste" );
  InsertActionAndAssignName( bar, cmEditDelete, "cmEditDelete" );
  InsertActionAndAssignName( bar, cmEditProperties, "cmEditProperties" );
  }




void SdWCommand::addViewCommands(QToolBar *bar)
  {
  bar->addSeparator();
  InsertActionAndAssignName( bar, cmViewGrid, "cmViewGrid" );
  InsertActionAndAssignName( bar, cmViewFill, "cmViewFill" );
  InsertActionAndAssignName( bar, cmModeTable[MD_ZOOM_IN], "cmModeTable_MD_ZOOM_IN" );
  InsertActionAndAssignName( bar, cmModeTable[MD_ZOOM_OUT], "cmModeTable_MD_ZOOM_OUT" );
  InsertActionAndAssignName( bar, cmModeTable[MD_ZOOM_WIN], "cmModeTable_MD_ZOOM_WIN" );
  InsertActionAndAssignName( bar, cmModeTable[MD_MEASUREMENT], "cmModeTable_MD_MEASUREMENT" );
  }





void SdWCommand::addDrawCommands(QToolBar *bar)
  {
  bar->addSeparator();
  InsertActionAndAssignName( bar, cmModeTable[MD_SELECT], "cmModeTable_MD_SELECT" );

  InsertActionAndAssignName( bar, cmModeTable[MD_LINE], "cmModeTable_MD_LINE" );
  InsertActionAndAssignName( bar, cmModeTable[MD_RECT], "cmModeTable_MD_RECT" );
  InsertActionAndAssignName( bar, cmModeTable[MD_FILL_RECT], "cmModeTable_MD_FILL_RECT" );
  InsertActionAndAssignName( bar, cmModeTable[MD_REGION], "cmModeTable_MD_REGION" );
  InsertActionAndAssignName( bar, cmModeTable[MD_FILL_REGION], "cmModeTable_MD_FILL_REGION" );
  InsertActionAndAssignName( bar, cmModeTable[MD_CIRCLE], "cmModeTable_MD_CIRCLE" );
  InsertActionAndAssignName( bar, cmModeTable[MD_FILL_CIRCLE], "cmModeTable_MD_FILL_CIRCLE" );
  InsertActionAndAssignName( bar, cmModeTable[MD_ARC], "cmModeTable_MD_ARC" );
  InsertActionAndAssignName( bar, cmModeTable[MD_TEXT], "cmModeTable_MD_TEXT" );
  InsertActionAndAssignName( bar, cmShowField, "cmShowField" );
  bar->addSeparator();
  }




void SdWCommand::activateModeBar( int barId, SdProjectItem *pitem )
  {
  if( barId >= 0 && barId < PB_LAST ) {
    //Loop through all panels, hide inactive panels, and display the active panel.
    for( int i = 0; i < PB_LAST; i++ )
      if( mBarTable[i] )
        mBarTable[i]->setVisible( i == barId );
    //Set the editable element in the active panel.
    mBarTable[barId]->setEditObject( pitem );
    }
  }



SdPropBar *SdWCommand::getModeBar(int barId)
  {
  if( barId >= 0 && barId < PB_LAST )
    return mBarTable[barId];
  return nullptr;
  }




//Get current visible barId
int SdWCommand::getModeBarId()
  {
  for( int i = 0; i < PB_LAST; i++ )
    if( mBarTable[i] && mBarTable[i]->isVisible() )
      return i;
  return PB_DEFAULT;
  }



QMenu *SdWCommand::getSelectMenu(SdClass objectClass)
  {
  if( objectClass & (dctSymbol | dctPart) ) return menuSelectSymPart;
  if( objectClass & (dctSheet) ) return menuSelectSheet;
  return menuSelect;
  }



template<typename PropBar>
static PropBar *buildPropBar( SdWMain *frame, const QString &title )
  {
  PropBar *bar = new PropBar( title );
  frame->addToolBar( bar );
  bar->setVisible(false);
  Q_ASSERT( SdWCommand::mBarTable[PropBar::mBarId] == nullptr );
  SdWCommand::mBarTable[PropBar::mBarId] = bar;
  bar->connect( bar, &PropBar::propChanged, frame, &SdWMain::cmPropertiesChange );
  return bar;
  }



void SdWCommand::createToolBars(SdWMain *frame)
  {
  //Main bar
  barMain = new QToolBar( QString("Main") );
  InsertActionAndAssignName( barMain, cmFileNew, "cmFileNew" );
  InsertActionAndAssignName( barMain, cmFileOpen, "cmFileOpen" );
  InsertActionAndAssignName( barMain, cmFileSave, "cmFileSave" );
  InsertActionAndAssignName( barMain, cmFilePrint, "cmFilePrint" );
  InsertActionAndAssignName( barMain, cmFileCloud, "cmFileCloud" );
  barMain->addSeparator();
  InsertActionAndAssignName( barMain, cmObjectNew, "cmObjectNew" );
  InsertActionAndAssignName( barMain, cmObjectEditEnable, "cmObjectEditEnable" );
  InsertActionAndAssignName( barMain, cmObjectEditDisable, "cmObjectEditDisable" );
  InsertActionAndAssignName( barMain, cmObjectPublic, "cmObjectPublic" );
  InsertActionAndAssignName( barMain, cmEditUndo, "cmEditUndo" );
  InsertActionAndAssignName( barMain, cmEditRedo, "cmEditRedo" );

  frame->addToolBar( barMain );








  //Symbol bar
  barSymbol = new QToolBar( QString("Symbol") );
  addEditCommands( barSymbol );
  addViewCommands( barSymbol );
  addDrawCommands( barSymbol );
  InsertActionAndAssignName( barSymbol, cmModeTable[MD_SYM_PIN], "cmModeTable_MD_SYM_PIN" );
  InsertActionAndAssignName( barSymbol, cmModeTable[MD_SYM_IDENT], "cmModeTable_MD_SYM_IDENT" );
  InsertActionAndAssignName( barSymbol, cmModeTable[MD_SYM_ORIGIN], "cmModeTable_MD_SYM_ORIGIN" );
  InsertActionAndAssignName( barSymbol, cmModeTable[MD_SYM_VALUE], "cmModeTable_MD_SYM_VALUE" );
  InsertActionAndAssignName( barSymbol, cmModeTable[MD_SYMBOL_FRAGMENT], "cmModeTable_MD_SYMBOL_FRAGMENT" );
  barSymbol->addSeparator();
  InsertActionAndAssignName( barSymbol, cmSymbolPartParam, "cmSymbolPartParam" );

  frame->addToolBar( barSymbol );







  //Part bar
  barPart = new QToolBar( QString("Part") );
  addEditCommands( barPart );
  addViewCommands( barPart );
  addDrawCommands( barPart );
  InsertActionAndAssignName( barPart, cmShowPads, "cmShowPads" );
  InsertActionAndAssignName( barPart, cmModeTable[MD_PART_PIN], "cmModeTable_MD_PART_PIN" );
  InsertActionAndAssignName( barPart, cmModeTable[MD_PART_IDENT], "cmModeTable_MD_PART_IDENT" );
  InsertActionAndAssignName( barPart, cmModeTable[MD_PART_ORIGIN], "cmModeTable_MD_PART_ORIGIN" );
  InsertActionAndAssignName( barPart, cmModeTable[MD_PART_VALUE], "cmModeTable_MD_PART_VALUE" );

  frame->addToolBar( barPart );





  //Part 3d bar
  barPart3d = new QToolBar( QString("Part3d") );
  InsertActionAndAssignName( barPart3d, cm3dShowPads, "cm3dShowPads" );
  InsertActionAndAssignName( barPart3d, cm3dShow2d, "cm3dShow2d" );
  InsertActionAndAssignName( barPart3d, cm3dTopView, "cm3dTopView" );
  barPart3d->addSeparator();
  InsertActionAndAssignName( barPart3d, cmModeTable[MD_3D_VIEW], "cmModeTable_MD_3D_VIEW" );
  barPart3d->addSeparator();
  InsertActionAndAssignName( barPart3d, cm3dProgram, "cm3dProgram" );
  barPart3d->addSeparator();
  InsertActionAndAssignName( barPart3d, cmModeTable[MD_3D_HORZ_MOVE], "cmModeTable_MD_3D_HORZ_MOVE" );
  InsertActionAndAssignName( barPart3d, cmModeTable[MD_3D_HORZ_ROTATE], "cmModeTable_MD_3D_HORZ_ROTATE" );
  InsertActionAndAssignName( barPart3d, cmModeTable[MD_3D_VERT_MOVE], "cmModeTable_MD_3D_VERT_MOVE" );
  InsertActionAndAssignName( barPart3d, cmModeTable[MD_3D_VERT_ROTATE], "cmModeTable_MD_3D_VERT_ROTATE" );

  frame->addToolBar( barPart3d );



  //Part 3d bar view
  barPart3dView = new QToolBar( QString("Part3dView") );
  InsertActionAndAssignName( barPart3dView, cm3dShowPads, "cm3dShowPads" );
  InsertActionAndAssignName( barPart3dView, cm3dShow2d, "cm3dShow2d" );
  InsertActionAndAssignName( barPart3dView, cm3dTopView, "cm3dTopView" );
  barPart3dView->addSeparator();
  InsertActionAndAssignName( barPart3dView, cm3dProgram, "cm3dProgram" );
  frame->addToolBar( barPart3dView );

  //Comp bar
  barComp = new QToolBar( QString("Component") );

  frame->addToolBar( barComp );



  //Sheet bar
  barSheet = new QToolBar( QString("Sheet") );
  addEditCommands( barSheet );
  addViewCommands( barSheet );
  addDrawCommands( barSheet );
  InsertActionAndAssignName( barSheet, cmModeTable[MD_FRAGMENT], "cmModeTable_MD_FRAGMENT" );
  InsertActionAndAssignName( barSheet, cmModeTable[MD_COMPONENT], "cmModeTable_MD_COMPONENT" );
  InsertActionAndAssignName( barSheet, cmModeTable[MD_NET], "cmModeTable_MD_NET" );
  InsertActionAndAssignName( barSheet, cmModeTable[MD_BUS], "cmModeTable_MD_BUS" );
  InsertActionAndAssignName( barSheet, cmModeTable[MD_DISCONNECT], "cmModeTable_MD_DISCONNECT" );
  InsertActionAndAssignName( barSheet, cmModeTable[MD_NET_NAME], "cmModeTable_MD_NET_NAME" );
  InsertActionAndAssignName( barSheet, cmModeTable[MD_SHEET_IDENT], "cmModeTable_MD_SHEET_IDENT" );
  InsertActionAndAssignName( barSheet, cmModeTable[MD_SHEET_VALUE], "cmModeTable_MD_SHEET_VALUE" );
  InsertActionAndAssignName( barSheet, cmModeTable[MD_NET_LIST], "cmModeTable_MD_NET_LIST" );

  frame->addToolBar( barSheet );



  //PCB bar
  barPcb = new QToolBar( QString("PCB") );
  addEditCommands( barPcb );
  addViewCommands( barPcb );
  addDrawCommands( barPcb );
  InsertActionAndAssignName( barPcb, cmShowRatNet, "cmShowRatNet" );
  InsertActionAndAssignName( barPcb, cmShowRuleErrors, "cmShowRuleErrors" );
  InsertActionAndAssignName( barPcb, cmRulesEdit, "cmRulesEdit" );
  InsertActionAndAssignName( barPcb, cmRulesCheck, "cmRulesCheck" );
  InsertActionAndAssignName( barPcb, cmPads, "cmPads" );
  InsertActionAndAssignName( barPcb, cmModeTable[MD_PART_IMP], "cmModeTable_MD_PART_IMP" );
  InsertActionAndAssignName( barPcb, cmModeTable[MD_MOVE_PART], "cmModeTable_MD_MOVE_PART" );
  InsertActionAndAssignName( barPcb, cmModeTable[MD_ROAD_ENTER], "cmModeTable_MD_ROAD_ENTER" );
  InsertActionAndAssignName( barPcb, cmModeTable[MD_ROAD_MOVE], "cmModeTable_MD_ROAD_MOVE" );
  InsertActionAndAssignName( barPcb, cmModeTable[MD_TRACE_DELETE], "cmModeTable_MD_TRACE_DELETE" );
  InsertActionAndAssignName( barPcb, cmModeTable[MD_POLYGON], "cmModeTable_MD_POLYGON" );
  InsertActionAndAssignName( barPcb, cmModeTable[MD_VIA_ENTER], "cmModeTable_MD_VIA_ENTER" );
  InsertActionAndAssignName( barPcb, cmModeTable[MD_PLATE_IDENT], "cmModeTable_MD_PLATE_IDENT" );
  InsertActionAndAssignName( barPcb, cmModeTable[MD_PLATE_VALUE], "cmModeTable_MD_PLATE_VALUE" );
  InsertActionAndAssignName( barPcb, cmModeTable[MD_PLATE_ORIGIN], "cmModeTable_MD_PLATE_ORIGIN" );
  InsertActionAndAssignName( barPcb, cmModeTable[MD_PART_HIGHLIGHT], "cmModeTable_MD_PART_HIGHLIGHT" );

  frame->addToolBar( barPcb );



  //View bar
  barView = new QToolBar( QString("View") );
  addViewCommands( barView );
  //barView->insertAction( nullptr, cmObjectEditEnable );

  frame->addToolBar( barView );


  //Help bar
  barHelp = new QToolBar( QString("Help") );
  InsertActionAndAssignName( barHelp, cmHelpHome, "cmHelpHome" );
  InsertActionAndAssignName( barHelp, cmHelpBackward, "cmHelpBackward" );
  InsertActionAndAssignName( barHelp, cmHelpForward, "cmHelpForward" );
  frame->addToolBar(barHelp);


  frame->addToolBarBreak();

  buildPropBar<SdPropBarLay>( frame, QStringLiteral("Default tool bar") );

  buildPropBar<SdPropBarLinear>( frame, QStringLiteral("Linear mode") );

  buildPropBar<SdPropBarTextual>( frame, QStringLiteral("Textual mode") );

  buildPropBar<SdPropBarSymPin>( frame, QStringLiteral("Sym pin") );

  buildPropBar<SdPropBarPartPin>( frame, QStringLiteral("Part pin") );

  buildPropBar<SdPropBarSymImp>( frame, QStringLiteral("Symbol implement") );

  buildPropBar<SdPropBarPartImp>( frame, QStringLiteral("Part implement") );

  buildPropBar<SdPropBarRoad>( frame, QStringLiteral("Road") );

  buildPropBar<SdPropBarPolygon>( frame, QStringLiteral("Polygon") );

  buildPropBar<SdPropBarWire>( frame, QStringLiteral("Wire") );

  auto pbar = buildPropBar<SdPropBarPartPlace>( frame, QStringLiteral("Part place") );
  pbar->connect( pbar, &SdPropBarPartPlace::partSelect, frame, &SdWMain::cmModePartSelect );

  buildPropBar<SdPropBarDefault>( frame, QStringLiteral("No selection") );

  buildPropBar<SdPropBarVia>( frame, QStringLiteral("Via") );

  for( int i = 0; i < MD_LAST; i++ )
    if( cmModeTable[i] )
      cmModeTable[i]->setCheckable(true);
  }



void SdWCommand::hideEditorContext()
  {
  //Hide all editor-referenced menu  [Погасить все редакторо-зависимые меню insert]
  cmMenuViewPart3d->setVisible(false);
  cmMenuInsertComp->setVisible(false);
  cmMenuInsertPart->setVisible(false);
  cmMenuInsertPart3d->setVisible(false);
  cmMenuInsertPcb->setVisible(false);
  cmMenuInsertSheet->setVisible(false);
  cmMenuInsertSymbol->setVisible(false);
  cmObjectEditEnable->setVisible(false);
  cmObjectEditDisable->setVisible(false);
  cmObjectPublic->setVisible(false);
  cmMenuRules->setVisible(false);

  //Погасить все редакторо-зависимые toolBars
  barComp->hide();
  barPart->hide();
  barPart3d->hide();
  barPart3dView->hide();
  barPcb->hide();
  barSheet->hide();
  barSymbol->hide();
  barView->hide();
  barHelp->hide();
  }




void SdWCommand::selectMode(int md)
  {
  //qDebug() << "selectMode" << md;
  for( int i = 0; i < MD_LAST; i++ )
    if( cmModeTable[i] )
      cmModeTable[i]->setChecked( i == md );
  }



void SdWCommand::__addMenuMap(const QString &id, QMenu *rootMenu, QAction *menuAction)
  {
  mMenuMap.insert( id, QObject::tr("Menu \"%1\":\"%2\"").arg(rootMenu->title(), menuAction->text() ) );
  }









QActionPtr SdWCommand::cmFileNew;
QActionPtr SdWCommand::cmFileOpen;
QActionPtr SdWCommand::cmFileClose;
QActionPtr SdWCommand::cmFileCloseAll;
QActionPtr SdWCommand::cmFileSave;
QActionPtr SdWCommand::cmFileSaveAs;
QActionPtr SdWCommand::cmFileSaveAll;
QActionPtr SdWCommand::cmFilePrint;
QActionPtr SdWCommand::cmFileExit;
QActionPtr SdWCommand::cmFileImportPis;
QActionPtr SdWCommand::cmFileImport;
QActionPtr SdWCommand::cmFileExport;
QActionPtr SdWCommand::cmFileExportGerber;
QActionPtr SdWCommand::cmFileStoreToLibrary;
QActionPtr SdWCommand::cmFileLoadFromLibrary;
QActionPtr SdWCommand::cmFileCloud;


QActionPtr SdWCommand::cmFilePrevious[PREVIOUS_FILES_COUNT];

QActionPtr SdWCommand::cmObjectNew;
QActionPtr SdWCommand::cmObjectLoad;
QActionPtr SdWCommand::cmObjectRename;
QActionPtr SdWCommand::cmObjectDelete;
QActionPtr SdWCommand::cmObjectCopy;
QActionPtr SdWCommand::cmObjectPaste;
QActionPtr SdWCommand::cmObjectCut;
QActionPtr SdWCommand::cmObjectDuplicate;
QActionPtr SdWCommand::cmObjectSort;
QActionPtr SdWCommand::cmObjectParam;
QActionPtr SdWCommand::cmObjectEditEnable;
QActionPtr SdWCommand::cmObjectEditDisable;
QActionPtr SdWCommand::cmObjectPublic;
QActionPtr SdWCommand::cmProjectParam;
QActionPtr SdWCommand::cmProjectUpgrade;

QActionPtr SdWCommand::cmEditUndo;
QActionPtr SdWCommand::cmEditRedo;
QActionPtr SdWCommand::cmEditCut;
QActionPtr SdWCommand::cmEditCopy;
QActionPtr SdWCommand::cmEditCopySvg;
QActionPtr SdWCommand::cmEditCopyWeb;
QActionPtr SdWCommand::cmEditPaste;
QActionPtr SdWCommand::cmEditDelete;
QActionPtr SdWCommand::cmEditSelectAll;
QActionPtr SdWCommand::cmEditSelectSave;
QActionPtr SdWCommand::cmEditUnSelect;
QActionPtr SdWCommand::cmEditFind;
QActionPtr SdWCommand::cmEditReplace;
QActionPtr SdWCommand::cmEditRotateGroup;
QActionPtr SdWCommand::cmEditProperties;
QActionPtr SdWCommand::cmEditCalculations;
QActionPtr SdWCommand::cmEditFragments;

QActionPtr SdWCommand::cmContextSelectItem[MCC_SELECT_ITEM_COUNT];
QActionPtr SdWCommand::cmContextComponentRotate;
QActionPtr SdWCommand::cmContextComponentFlip;
QActionPtr SdWCommand::cmContextGroupRotate;


QActionPtr SdWCommand::cmViewProject;
QActionPtr SdWCommand::cmView3d;
QActionPtr SdWCommand::cmViewMirror;
QActionPtr SdWCommand::cmViewFill;
QActionPtr SdWCommand::cmViewNets;
QActionPtr SdWCommand::cmViewGrid;
QActionPtr SdWCommand::cmViewLayers;

QActionPtr SdWCommand::cm3dMaster;
QActionPtr SdWCommand::cm3dProgram;
QActionPtr SdWCommand::cm3dImportStl;
QActionPtr SdWCommand::cm3dImportStep;
QActionPtr SdWCommand::cm3dImportVrml;
QActionPtr SdWCommand::cm3dShow2d;
QActionPtr SdWCommand::cm3dShowPads;
QActionPtr SdWCommand::cm3dTopView;

QActionPtr SdWCommand::cmNetSetup;
QActionPtr SdWCommand::cmModeLink;
QActionPtr SdWCommand::cmModeNetList;
QActionPtr SdWCommand::cmModePack;
QActionPtr SdWCommand::cmPads;

QActionPtr SdWCommand::cmShowRatNet;
QActionPtr SdWCommand::cmShowRuleErrors;
QActionPtr SdWCommand::cmRenumeration;
QActionPtr SdWCommand::cmShowField;
QActionPtr SdWCommand::cmShowPads;
QActionPtr SdWCommand::cmPcbStratum;

QActionPtr SdWCommand::cmRulesEdit;
QActionPtr SdWCommand::cmRulesCheck;
QActionPtr SdWCommand::cmRulesErrorNext;

QActionPtr SdWCommand::cmSymbolPartParam;

QActionPtr SdWCommand::cmSheetExpression;

QActionPtr SdWCommand::cmOption;

QActionPtr SdWCommand::cmTools;

QActionPtr SdWCommand::cmHelpContents;
QActionPtr SdWCommand::cmHelpIndex;
QActionPtr SdWCommand::cmHelpAbout;
QActionPtr SdWCommand::cmHelpRegistration;
QActionPtr SdWCommand::cmHelpHome;
QActionPtr SdWCommand::cmHelpBackward;
QActionPtr SdWCommand::cmHelpForward;

QActionPtr SdWCommand::cmGuiderDialog;
QActionPtr SdWCommand::cmGuiderRecord;
QActionPtr SdWCommand::cmGuiderNextStep;

QMenu *SdWCommand::menuFile;
QMenu *SdWCommand::menuFilePrevious;
QMenu *SdWCommand::menuObject;
QMenu *SdWCommand::menuEdit;
QMenu *SdWCommand::menuView;
QMenu *SdWCommand::menuViewPart3d;
QMenu *SdWCommand::menuDraw;
QMenu *SdWCommand::menuInsertSymbol;
QMenu *SdWCommand::menuInsertSheet;
QMenu *SdWCommand::menuInsertPart;
QMenu *SdWCommand::menuInsertPart3d;
QMenu *SdWCommand::menuInsertPcb;
QMenu *SdWCommand::menuInsertComp;
QMenu *SdWCommand::menuInstruments;
QMenu *SdWCommand::menuHelp;
QMenu *SdWCommand::menuRules;

QMenu *SdWCommand::menuSelect;
QMenu *SdWCommand::menuSelectSymPart;
QMenu *SdWCommand::menuSelectSheet;

QActionPtr SdWCommand::cmMenuInsertSymbol;
QActionPtr SdWCommand::cmMenuInsertSheet;
QActionPtr SdWCommand::cmMenuInsertPart;
QActionPtr SdWCommand::cmMenuInsertPart3d;
QActionPtr SdWCommand::cmMenuInsertPcb;
QActionPtr SdWCommand::cmMenuInsertComp;
QActionPtr SdWCommand::cmMenuRules;
QActionPtr SdWCommand::cmMenuViewPart3d;

QToolBar *SdWCommand::barMain;
QToolBar *SdWCommand::barSymbol;
QToolBar *SdWCommand::barPart;
QToolBar *SdWCommand::barPart3d;
QToolBar *SdWCommand::barPart3dView;
QToolBar *SdWCommand::barComp;
QToolBar *SdWCommand::barSheet;
QToolBar *SdWCommand::barPcb;
QToolBar *SdWCommand::barView;
QToolBar *SdWCommand::barHelp;


//Full mode action table
QActionPtr   SdWCommand::cmModeTable[MD_LAST];

//Full list mode tool bars
SdPropBarPtr SdWCommand::mBarTable[PB_LAST];

QMap<QString,QString> SdWCommand::mMenuMap;
