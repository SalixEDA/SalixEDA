#include "SdImportKiCadLib.h"
#include "import/SdImportManager.h"

#include <QObject>

static SdImportKiCadLib sdImportKiCadLib;

SdImportKiCadLib::SdImportKiCadLib()
  {
  SdImportManager::instance()->registerImport( QObject::tr("Import KiCad part and symbols into existing project"),
                                               QString{},
                                               QObject::tr("Select KiCad mod and sym files"),
                                               QObject::tr("KiCad files (*.kicad_mod *.kicad_sym)"),
                                               true,
                                               &SdImportKiCadLib::import
                                               );
  }

bool SdImportKiCadLib::import(QWidget *parent, const QStringList &fileList, SdProject *prj, bool usedExisting)
  {
  for( auto &fileName : std::as_const(fileList) ) {
    if( fileName.endsWith(".kicad_mod") ) {
      //Import from kicad mod file
  //     SdScanerKiCad reader;
  //     //reader.readFile( "/home/usik/info/kicad/D_01005_0402Metric.kicad_mod" );
  //     if( !reader.readFile( title ) ) {
  //       QMessageBox::warning( this, tr("Error"), tr("Can't read file '%1'").arg(title) );
  //       return;
  //       }
  //     SdProject *prj = new SdProject();
  //     reader.parseTop( prj );

  //     mWProjectList->fileOpen( title.left( title.length() - 10 ) + SD_BINARY_EXTENSION, prj );
  //     }
      }
    }
  return true;
  }
