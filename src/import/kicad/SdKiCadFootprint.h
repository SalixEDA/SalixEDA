#ifndef SDKICADFOOTPRINT_H
#define SDKICADFOOTPRINT_H

#include "SdKiCad.h"

class SdProject;
class SdPItemPart;
struct SdPropLine;
struct SdPropText;

class SdKiCadFootprint : public SdSeGroup<SdKiCadFootprint>
  {
    SdProject   *mDestProject;
    SdPItemPart *mPart;

    QString          mName;
    SdSeInt          mVersion;
    SdSeStringOrName mGenerator;
    SdSeString       mLayer;
    SdSeString       mDescr;
    SdSeString       mTags;
    SdKiCadFpAttr    mAttr;
  public:
    SdKiCadFootprint( SdProject *prj );

    void prolog( SdScanerSe* );

    void epilog( SdScanerSe* );

    void fp_text( SdScanerSe *scaner );

    void fp_line( SdScanerSe *scaner );

    void fp_rect( SdScanerSe *scaner );

    void fp_circle( SdScanerSe *scaner );

    void fp_arc( SdScanerSe *scaner );

    void fp_poly( SdScanerSe *scaner );

    void fp_pad( SdScanerSe *scaner );

    inline static const SdSeSerialList<SdKiCadFootprint> serialList =
    {
      sdSeScanString<SdKiCadFootprint, &SdKiCadFootprint::mName>
    };

    inline static const SdSeTokenMap<SdKiCadFootprint> tokenMap =
    {
      {"version",   sdSeParse<SdKiCadFootprint, &SdKiCadFootprint::mVersion>  },
      {"generator", sdSeParse<SdKiCadFootprint, &SdKiCadFootprint::mGenerator>  },
      {"descr",     sdSeParse<SdKiCadFootprint, &SdKiCadFootprint::mDescr>  },
      {"tags",      sdSeParse<SdKiCadFootprint, &SdKiCadFootprint::mTags>  },
      {"layer",     sdSeParse<SdKiCadFootprint, &SdKiCadFootprint::mLayer>  },
      {"attr",      sdSeParse<SdKiCadFootprint, &SdKiCadFootprint::mAttr>  },
      {"fp_text",   sdSeParseHandler<SdKiCadFootprint, &SdKiCadFootprint::fp_text>  },
      {"fp_line",   sdSeParseHandler<SdKiCadFootprint, &SdKiCadFootprint::fp_line>  },
      {"fp_rect",   sdSeParseHandler<SdKiCadFootprint, &SdKiCadFootprint::fp_rect>  },
      {"fp_circle", sdSeParseHandler<SdKiCadFootprint, &SdKiCadFootprint::fp_circle>  },
      {"fp_arc",    sdSeParseHandler<SdKiCadFootprint, &SdKiCadFootprint::fp_arc>  },
      {"fp_poly",   sdSeParseHandler<SdKiCadFootprint, &SdKiCadFootprint::fp_poly>  },
      {"fp_pad",    sdSeParseHandler<SdKiCadFootprint, &SdKiCadFootprint::fp_pad>  },
    };

    void fillPropLine( SdPropLine &prop, const SdKiCadStroke &stroke, const SdSeString &layer );

    void fillPropText( SdPropText &prop, const SdKiCadFpEffects &effects, const SdSeStringOrName &layer );

    static QString layerMap( const QString &kiCadLayer );
  };

#endif // SDKICADFOOTPRINT_H
