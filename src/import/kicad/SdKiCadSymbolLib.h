#ifndef SDKICADSYMBOLLIB_H
#define SDKICADSYMBOLLIB_H

#include "SdKiCad.h"

class SdProject;

class SdKiCadSymbolLib : public SdSeGroup<SdKiCadSymbolLib>
  {
    SdProject *mDestProject;
  public:
    SdKiCadSymbolLib( SdProject *prj );
  }

#endif // SDKICADSYMBOLLIB_H
