/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Graph editor for schematic sheet
*/
#include "SdWEditorGraphSheet.h"
#include "SdDParamEditor.h"
#include "SdWCommand.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphLinear.h"
#include "objects/SdGraphText.h"
#include "SdPMasterList.h"
#include "SdPExport_Bom.h"
#include "SdPExport_Dxf.h"
#include "modes/SdModeCSymImp.h"
#include "modes/SdModeCNetWire.h"
#include "modes/SdModeCLinearArea.h"
#include "modes/SdModeCNetName.h"
#include "modes/SdModeWireDisconnect.h"
#include "modes/SdModeCBus.h"
#include "modes/SdModeCFragment.h"
#include "modes/SdModeTBrowseSheetPart.h"
#include "modes/SdModeSelect.h"
#include "modes/SdModeCSheetIdentMove.h"
#include "modes/SdModeCSheetValueMove.h"
#include "modes/SdModeCScript.h"
#include "modes/SdModeCNetPinsList.h"
#include "windows/SdDExpressionEdit.h"
#include "windows/SdDGetObject.h"

#include <QMessageBox>
#include <QDebug>

SdWEditorGraphSheet::SdWEditorGraphSheet(SdPItemSheet *sch, QWidget *parent) :
  SdWEditorGraph( sch, parent ),
  mSheet(sch)
  {
  mGrid.set( 250, 250 );
  }




SdProjectItem *SdWEditorGraphSheet::getProjectItem() const
  {
  return mSheet;
  }




void SdWEditorGraphSheet::onActivateEditor()
  {
  SdWEditorGraph::onActivateEditor();

  //Activate menu
  SdWCommand::cmMenuInsertSheet->setVisible(true);

  //Activate tool bar
  SdWCommand::barSheet->show();
  }




void SdWEditorGraphSheet::setSelectionStatus(bool status)
  {
  SdWEditorGraph::setSelectionStatus( status );
  //Special case for selection mode when only one component selected
  status = status && mMode == mSelect && mSelect->isOneSymbolSelected();
  SdWCommand::cmEditCalculations->setEnabled( status );
  SdWCommand::cmEditFragments->setEnabled( status );
  }




//Sheet param insertion mode
//void SdWEditorGraphSheet::cmModeParam()
//  {
//  modeSet( new SdModeCParam( this, mSheet ) );
//  }





//Fragment insertion mode
void SdWEditorGraphSheet::cmModeFragment()
  {
  modeSet( new SdModeCFragment( this, mSheet ) );
  }


//Component insertion mode
void SdWEditorGraphSheet::cmModeComponent()
  {
  modeSet( new SdModeCSymImp( this, mSheet ) );
  }



//Sheet net wire insertion mode
void SdWEditorGraphSheet::cmModeNet()
  {
  modeSet( new SdModeCNetWire( this, mSheet ) );
  }




//Sheet bus insertion mode
void SdWEditorGraphSheet::cmModeBus()
  {
  modeSet( new SdModeCBus( this, mSheet )  );
  }




//Sheet net wire disconnect from pin mode
void SdWEditorGraphSheet::cmModeDisconnect()
  {
  modeSet( new SdModeWireDisconnect( this, mSheet )  );
  }




//Sheet pcb area insertion mode
void SdWEditorGraphSheet::cmModePcbArea()
  {
  modeSet( new SdModeCLinearArea( this, mSheet ) );
  }



//Sheet net name insertion mode
void SdWEditorGraphSheet::cmModeNetName()
  {
  modeSet( new SdModeCNetName( this, mSheet ) );
  }





//Browse part implement in sheet
void SdWEditorGraphSheet::cmModeBrowse(SdProjectItem *plate)
  {
  modeCall( new SdModeTBrowseSheetPart( this, mSheet, plate ) );
  }




void SdWEditorGraphSheet::cmModeScript()
  {
  modeSet( new SdModeCScript( this, mSheet )  );
  }




//Edit properties of selected objects
void SdWEditorGraphSheet::cmEditProperties()
  {
  //Only for selecting mode
  if( mMode == mSelect && mSelect != nullptr ) {
    //Find first symbol implement
    SdObject *symImp = nullptr;
    mSelect->getFragment()->forEach( dctSymImp, [&symImp] (SdObject *obj) -> bool {
      symImp = obj;
      return false;
      });
    //Use symImp params for init param editor dialog
    SdPtr<SdGraphSymImp> sym(symImp);
    if( sym.isValid() ) {
      SdDParamEditor editor( tr("Component params"), sym->paramTable(), getProject(), true, false, this );
      if( editor.exec() ) {
        //Change params for all selected items
        SdUndo *undo = getProject()->getUndo();
        undo->begin( tr("Param change"), getProjectItem(), false );
        mSelect->getFragment()->forEach( dctSymImp, [&editor,undo] (SdObject *obj) -> bool {
          SdPtr<SdGraphSymImp> imp(obj);
          if( imp.isValid() )
            imp->paramTableSet( editor.paramTable(), undo, nullptr );
          return true;
          });
        dirtyProject();
        }
      }
    else QMessageBox::warning( this, tr("Error!"), tr("Parameters edit available only for component. No component selected. Select components and try again.") );
    }
  }



//!
//! \brief cmEditCalculations Find and display possible calculations for selected component
//!
void SdWEditorGraphSheet::cmEditCalculations()
  {

  }




//!
//! \brief cmEditFragments Find, display and enable replace possible fragments for selected component
//!
void SdWEditorGraphSheet::cmEditFragments()
  {

  }



void SdWEditorGraphSheet::cmModeNetList()
  {
  modeSet( new SdModeCNetPinsList( this, mSheet )  );
  }




void SdWEditorGraphSheet::cmRenumeration()
  {
  //Perform renumeration
  getProject()->renumeration();

  //Update visual
  dirtyCashe();
  dirtyProject();
  update();
  }





//Move component reference
void SdWEditorGraphSheet::cmModeReferenceMove()
  {
  modeSet( new SdModeCSheetIdentMove( this, mSheet ) );
  }





//Move component value
void SdWEditorGraphSheet::cmModeValueMove()
  {
  modeSet( new SdModeCSheetValueMove( this, mSheet ) );
  }




//Export command
void SdWEditorGraphSheet::cmFileExport()
  {
  //Wizard
  QWizard wizard(this);
  //Fill it with pages
  SdPMasterList *master = new SdPMasterList( tr("Export sheet"), tr("Select export master"), &wizard );
  wizard.setPage( 0,   master );
  wizard.setPage( 1,   new SdPExport_Bom( mSheet, 1, master, &wizard) );
  wizard.setPage( 2,   new SdPExport_Dxf( mSheet, 2, master, &wizard) );
  wizard.exec();
  }




//!
//! \brief cmFormInsert Insert and replace form
//!
void SdWEditorGraphSheet::cmFormInsert()
  {
  QString sheetName;
  SdProject *project = SdDGetObject::getProject( sheetName, QObject::tr("Select fragment to insert"), this, "form" );
  if( project != nullptr ) {
    //At first step we remove previous form
    SdSelector selector;
    mSheet->forEach( dctLines|dctText, [&selector](SdObject *obj) ->bool {
      //Convert object to Graph object
      SdPtr<SdGraphLinear> ptr(obj);
      if( ptr.isValid() && ptr->isMatchId(LID0_FORM) )
        selector.insert( ptr.ptr() );

      SdPtr<SdGraphText> tp(obj);
      if( tp.isValid() && tp->isMatchId(LID0_FORM) )
        selector.insert( tp.ptr() );
      return true; //Continue scan
      } );

    //Delete all selected objects
    SdUndo *undo = mSheet->getUndo();
    undo->begin( QObject::tr("Replace form"), mSheet, false );
    selector.forEach( dctAll, [undo] (SdObject *obj) ->bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr )
        graph->deleteObject( undo );
      return true;
      });
    selector.removeAll();

    //Insert new form
    modeSet( new SdModeCFragment( this, mSheet, project, sheetName ) );
    }
  }



