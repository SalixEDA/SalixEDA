/*
Project "Electronic schematic and pcb CAD"

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Base object for 3d graphics
*/
#include "Sd3dGraph.h"

Sd3dGraph::Sd3dGraph() :
  SdGraph()
  {

  }





SdClass Sd3dGraph::getClass() const
  {
  return dct3D;
  }
