#include "SdModeCNetPinsList.h"
#include "objects/SdGraphNetPinsList.h"


SdModeCNetPinsList::SdModeCNetPinsList(SdWEditorGraph *editor, SdProjectItem *obj)
  : SdMode( editor, obj )
  {

  }


int SdModeCNetPinsList::getPropBarId() const
  {
  return PB_TEXT;
  }




void SdModeCNetPinsList::enterPoint(SdPoint p)
  {
  //Find object behind enter point
  SdGraphNetPinsList *pgraph = nullptr;
  mObject->forEach( dctText, [&pgraph,p]( SdObject *obj ) {
    SdPtr<SdGraphNetPinsList> graph(obj);
    if( graph.isValid() && graph->behindCursor(p) ) {
      pgraph = graph.ptr();
      return false;
      }
    return true;
    });

  if( pgraph == nullptr ) {
    //Insert new net pins list
    //Create and insert new Script object
    addPic( new SdGraphScript( edit.scriptGet(), p, *sdGlobalProp->propText(mObject->getClass()) ), QStringLiteral("Append new script")  );

    }
  }




void SdModeCNetPinsList::cancelPoint(SdPoint)
  {
  cancelMode();
  }




void SdModeCNetPinsList::movePoint(SdPoint)
  {
  }




QString SdModeCNetPinsList::getStepHelp() const
  {
  return QObject::tr("Enter point to insert or edit net pins list");
  }




QString SdModeCNetPinsList::getModeThema() const
  {
  return QString( MODE_HELP "ModeCNetPinsList.htm" );
  }




QString SdModeCNetPinsList::getStepThema() const
  {
  return getModeThema();
  }




int SdModeCNetPinsList::getCursor() const
  {
  return CUR_SCRIPT;
  }




int SdModeCNetPinsList::getIndex() const
  {
  return MD_NET_LIST;
  }
