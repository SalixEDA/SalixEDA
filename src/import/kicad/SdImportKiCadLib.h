#ifndef SDIMPORTKICADLIB_H
#define SDIMPORTKICADLIB_H

#include "import/SdImportManager.h"

class SdImportKiCadLib
  {
  public:
    SdImportKiCadLib();

    static bool import( QWidget *parent, const QStringList &fileList, SdProject *prj, bool usedExisting );
  };

#endif // SDIMPORTKICADLIB_H
