/*
Project "Electronic schematic and pcb CAD"

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description

*/
#include "SdConverterOffset.h"


SdConverterOffset::SdConverterOffset(SdPoint offset) :
  mOffset(offset)
  {

  }


QTransform SdConverterOffset::getMatrix()
  {
  return QTransform::fromTranslate( mOffset.x(), mOffset.y() );
  }

