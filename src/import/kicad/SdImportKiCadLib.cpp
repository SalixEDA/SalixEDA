#include "SdImportKiCadLib.h"
#include "SdScanerKiCad.h"
#include "SdKiCadFootprint.h"
#include "SdKiCadSymbolLib.h"
#include "import/SdImportManager.h"
#include "objects/SdProject.h"

#include <QObject>
#include <QMessageBox>

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
  if( usedExisting ) {
    prj->getUndo()->begin( QObject::tr("Import"), nullptr, false );
    }
  for( auto &fileName : std::as_const(fileList) ) {
    if( fileName.endsWith(".kicad_mod") ) {
      //Import from kicad mod file
      SdScanerSe scaner;
      if( scaner.readFile( fileName ) ) {
        //Token map
        static SdSeTokenMap<SdProject>
            tokenMap( { { QString("footprint"), sdSeTopParse<SdKiCadFootprint> },
                        { QString("kicad_symbol_lib"), sdSeTopParse<SdKiCadSymbolLib> }
                      } );

        //Parse full file
        scaner.tokenNext();
        while( !scaner.isEndOfScanOrError() )
          scaner.parseContent( tokenMap, prj );

        if( !scaner.errorGet().isEmpty() ) {
          QMessageBox::warning( parent, QObject::tr("Error!"), QObject::tr("Errors when parse file '%1': %2").arg( fileName, scaner.errorGet() ) );
          if( usedExisting )
            prj->getUndo()->undoStep();
          return false;
          }
        }
      else {
        QMessageBox::warning( parent, QObject::tr("Error!"), QObject::tr("Can't read file '%1'").arg(fileName) );
        if( usedExisting )
          prj->getUndo()->undoStep();
        return false;
        }
      }
    }
  return true;
  }
