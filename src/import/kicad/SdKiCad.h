/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
*/
#ifndef SDKICAD_H
#define SDKICAD_H

#include "import/SdScanerSe.h"

inline constexpr int SdKiCadFpScale = 1000;
inline constexpr int SdKiCadFpAngleScale = 1000;


/*


struct SdKiCadPointWithAngle : public SdKiCadValidable
  {
    QPointF mPoint;           //!< 2d point
    double  mAngle   = 0;     //!< Angle of rotation

    void scan( SdScanerKiCad *scaner )
      {
      mPoint.setX( scaner->tokenNeedValueDouble() );
      mPoint.setY( scaner->tokenNeedValueDouble() );
      mAngle = scaner->tokenNeedValueDouble();
      scaner->tokenNeedClose();
      mPresent = true;
      }
  };





struct SdKiCadSdPointWithAngle : public SdKiCadValidable
  {
    QPoint mPoint;           //!< 2d point
    int    mAngle   = 0;     //!< Angle of rotation

    void scan( SdScanerKiCad *scaner )
      {
      mPoint.setX( scaner->tokenNeedCoord() );
      mPoint.setY( scaner->tokenNeedCoord() );
      mAngle = scaner->tokenNeedAngle();
      scaner->tokenNeedClose();
      mPresent = true;
      }
  };


*/














struct SdKiCadYesNo : public SdSeSelector<SdKiCadYesNo>
  {
    SdKiCadYesNo( bool v ) : SdSeSelector<SdKiCadYesNo>(v ? 1 : 0) {}

    inline static const SdSeSelectorMap selectorMap = {
      { "yes", 1 },
      { "no",  0 }
    };

    bool isYes() const { return selector() == 1; }
  };





struct SdKiCadStrokeType : public SdSeSelector<SdKiCadStrokeType>
  {
    enum {
      typeNone,
      typeSolid,
      typeDashed,
      typeDotted,
      typeDashDot,
      typeDashDotDot,
    };

    inline static const SdSeSelectorMap selectorMap = {
      { "none",         typeNone },
      { "solid",        typeSolid },
      { "dashed",       typeDashed },
      { "dotted",       typeDotted },
      { "dash_dot",     typeDashDot },
      { "dash_dot_dot", typeDashDotDot }
    };
  };



struct SdKiCadFillType : public SdSeSelector<SdKiCadFillType>
  {
    enum {
      typeNone,
      typeSolid
    };

    inline static const SdSeSelectorMap selectorMap = {
      { "none",  typeNone },
      { "solid", typeSolid }
    };
  };



struct SdKiCadFpAttr : public SdSeSelector<SdKiCadFpAttr>
  {
    bool mExcludedFromPosFiles = false;
    bool mExcludedFromBom      = false;
    enum {
      fpaNone,
      fpaSmd,
      fpaTroughHole,
      fpaBoardOnly
    };
    inline static const QMap<QString,int> selectorMap = {
      { "none",         fpaNone },
      { "smd",          fpaSmd },
      { "through_hole", fpaTroughHole },
      { "board_only",   fpaBoardOnly }
    };

    inline static const SdSeTokenMap<SdKiCadFpAttr> tokenMap =
    {
      { "excluded_from_pos_files", sdSeParseFlag<SdKiCadFpAttr, &SdKiCadFpAttr::mExcludedFromPosFiles> },
      { "excluded_from_bom",       sdSeParseFlag<SdKiCadFpAttr, &SdKiCadFpAttr::mExcludedFromBom> },
    };

  };





struct SdKiCadStroke : public SdSeGroup<SdKiCadStroke>
  {
    SdSeIntScaled<SdKiCadFpScale> mWidth;
    SdKiCadStrokeType             mType;

    inline static SdSeTokenMap<SdKiCadStroke> tokenMap = {
      { "width", sdSeParse<SdKiCadStroke,&SdKiCadStroke::mWidth> },
      { "type",  sdSeParse<SdKiCadStroke,&SdKiCadStroke::mType> }
    };

    bool isValid() const { return mWidth.isValid() && mType.isValid(); }
  };



/*

struct SdKiCadCircle : public SdKiCadGrouped<SdKiCadCircle>
  {
    SdKiCadSdPoint mCenter;
    SdKiCadSdPoint mEnd;
    SdKiCadStroke  mStroke;
    SdKiCadString  mLayer;
    SdKiCadYesNo   mFilled;

    inline static SdKiCadTokenMap<SdKiCadCircle> tokenMap = {
      { "center", sdKiCad<SdKiCadCircle,&SdKiCadCircle::mCenter> },
      { "end",    sdKiCad<SdKiCadCircle,&SdKiCadCircle::mEnd> },
      { "stroke", sdKiCad<SdKiCadCircle,&SdKiCadCircle::mStroke> },
      { "layer",  sdKiCad<SdKiCadCircle,&SdKiCadCircle::mLayer> },
      { "fill",   sdKiCad<SdKiCadCircle,&SdKiCadCircle::mFilled> }
    };

    bool isValid() const { return mCenter.isValid() && mEnd.isValid() && mLayer.isValid() && (mFilled.isYes() || mStroke.isValid()); }
  };



struct SdKiCadZoneConnection : public SdKiCadMap<SdKiCadZoneConnection>
  {
    enum {
      padNone,
      padThermal,
      padSolid
    };

    inline static const QMap<QString,int> mTokenMap = {
        { "padNone",    padNone },
        { "padThermal", padThermal },
        { "padSolid",   padSolid }
      };
  };




struct SdKiCadPad : public SdKiCadGrouped<SdKiCadPad>
  {
    SdKiCadSdPointWithAngle mPosWithRotation;
    SdKiCadSdPoint          mSize;
    SdKiCadCoord            mDrill;
    SdKiCadStringList       mLayerList;
    SdKiCadDouble           mRoundRectRatio;
    SdKiCadCoord            mSolderMaskMargin;
    SdKiCadCoord            mSolderPasteMargin;
    SdKiCadDouble           mSolderPasteRatio;
    SdKiCadZoneConnection   mZoneConnection;

    inline static const SdKiCadTokenMap<SdKiCadPad> tokenMap = {
      { "at",                  sdKiCad<SdKiCadPad,&SdKiCadPad::mPosWithRotation> },
      { "size",                sdKiCad<SdKiCadPad,&SdKiCadPad::mSize> },
      { "drill",               sdKiCad<SdKiCadPad,&SdKiCadPad::mDrill> },
      { "layers",              sdKiCad<SdKiCadPad,&SdKiCadPad::mLayerList> },
      { "roundrect_rratio",    sdKiCad<SdKiCadPad,&SdKiCadPad::mRoundRectRatio> },
      { "solder_mask_margin",  sdKiCad<SdKiCadPad,&SdKiCadPad::mSolderMaskMargin> },
      { "solder_paste_margin", sdKiCad<SdKiCadPad,&SdKiCadPad::mSolderPasteMargin> },
      { "solder_paste_ratio",  sdKiCad<SdKiCadPad,&SdKiCadPad::mSolderPasteRatio> },
      { "zone_connection",     sdKiCad<SdKiCadPad,&SdKiCadPad::mZoneConnection> }
    };

  };

*/


struct SdKiCadFpTextAt : public SdSeGroup<SdKiCadFpTextAt>
  {
    int mPosX;
    int mPosY;
    int mAngle    = 0;
    int mUnlocked = -1;

    inline static const SdSeSerialList<SdKiCadFpTextAt> serialList = {
      sdSeScanIntScaled<SdKiCadFpTextAt, &SdKiCadFpTextAt::mPosX, SdKiCadFpScale >,
      sdSeScanIntScaled<SdKiCadFpTextAt, &SdKiCadFpTextAt::mPosY, SdKiCadFpScale >,
      sdSeScanIntScaled<SdKiCadFpTextAt, &SdKiCadFpTextAt::mAngle, SdKiCadFpAngleScale, SdSeOptional >,
      sdSeScanSelector<SdKiCadFpTextAt, &SdKiCadFpTextAt::mUnlocked, SdSeOptional>( {"unlocked"} )
    };
  };

struct SdKiCadFpTextSize : public SdSeGroup<SdKiCadFpTextSize>
  {
    int mSizeW;
    int mSizeH;

    inline static const SdSeSerialList<SdKiCadFpTextSize> serialList = {
      sdSeScanIntScaled<SdKiCadFpTextSize, &SdKiCadFpTextSize::mSizeW, SdKiCadFpScale >,
      sdSeScanIntScaled<SdKiCadFpTextSize, &SdKiCadFpTextSize::mSizeH, SdKiCadFpScale >,
      };
  };

struct SdKiCadFpTextFont : public SdSeGroup<SdKiCadFpTextFont>
  {
    SdKiCadFpTextSize             mSize;
    SdSeIntScaled<SdKiCadFpScale> mThickness;
    SdSeDouble                    mWidth = 1.0;
    SdKiCadYesNo                  mItalic = false;
    bool                          mBold = false;

    inline static const SdSeTokenMap<SdKiCadFpTextFont> tokenMap = {
      {"size",      sdSeParse<SdKiCadFpTextFont, &SdKiCadFpTextFont::mSize>},
      {"thickness", sdSeParse<SdKiCadFpTextFont, &SdKiCadFpTextFont::mThickness>},
      {"width",     sdSeParse<SdKiCadFpTextFont, &SdKiCadFpTextFont::mWidth>},
      {"italic",    sdSeParse<SdKiCadFpTextFont, &SdKiCadFpTextFont::mItalic>},
      //Flags
      {"bold",      sdSeParseFlag<SdKiCadFpTextFont, &SdKiCadFpTextFont::mBold>},
      };
  };


struct SdKiCadFpTextJustify : public SdSeGroup<SdKiCadFpTextJustify>
  {
    bool mLeft   = false;
    bool mRight  = false;
    bool mBottom = false;
    bool mTop    = false;
    bool mMirror = false;

    inline static const SdSeTokenMap<SdKiCadFpTextJustify> tokenMap = {
      {"left",   sdSeParseFlag<SdKiCadFpTextJustify, &SdKiCadFpTextJustify::mLeft>},
      {"right",  sdSeParseFlag<SdKiCadFpTextJustify, &SdKiCadFpTextJustify::mRight>},
      {"bottom", sdSeParseFlag<SdKiCadFpTextJustify, &SdKiCadFpTextJustify::mBottom>},
      {"top",    sdSeParseFlag<SdKiCadFpTextJustify, &SdKiCadFpTextJustify::mTop>},
      {"mirro",  sdSeParseFlag<SdKiCadFpTextJustify, &SdKiCadFpTextJustify::mMirror>},
      };
  };

struct SdKiCadFpEffects : public SdSeGroup<SdKiCadFpEffects>
  {
    SdKiCadFpTextFont    mFont;
    SdKiCadFpTextJustify mJustify;

    inline static const SdSeTokenMap<SdKiCadFpEffects> tokenMap = {
      {"font",    sdSeParse<SdKiCadFpEffects, &SdKiCadFpEffects::mFont>},
      {"justify", sdSeParse<SdKiCadFpEffects, &SdKiCadFpEffects::mJustify>},
      };
  };



struct SdKiCadFpText : public SdSeGroup<SdKiCadFpText>
  {
    QString          mType;
    QString          mText;

    SdKiCadFpTextAt  mAt;
    SdSeStringOrName mLayer;
    SdKiCadYesNo     mHide = false;
    SdKiCadFpEffects mEffects;


    inline static const SdSeSerialList<SdKiCadFpText> serialList = {
      sdSeScanName<SdKiCadFpText, &SdKiCadFpText::mType>,
      sdSeScanStringOrName<SdKiCadFpText, &SdKiCadFpText::mText >,
      };

    inline static const SdSeTokenMap<SdKiCadFpText> tokenMap = {
      {"at",      sdSeParse<SdKiCadFpText, &SdKiCadFpText::mAt>},
      {"layer",   sdSeParse<SdKiCadFpText, &SdKiCadFpText::mLayer>},
      {"hide",    sdSeParse<SdKiCadFpText, &SdKiCadFpText::mHide>},
      {"effects", sdSeParse<SdKiCadFpText, &SdKiCadFpText::mEffects>},
      };

  };




using SdKiCadFpSdPoint = SdSeSdPoint<SdKiCadFpScale>;


struct SdKiCadFpLine : public SdSeGroup<SdKiCadFpLine>
  {
    SdKiCadFpSdPoint mStart;
    SdKiCadFpSdPoint mEnd;
    SdKiCadStroke    mStroke;
    SdSeString       mLayer;

    inline static SdSeTokenMap<SdKiCadFpLine> tokenMap = {
      { "start",  sdSeParse<SdKiCadFpLine,&SdKiCadFpLine::mStart> },
      { "end",    sdSeParse<SdKiCadFpLine,&SdKiCadFpLine::mEnd> },
      { "stroke", sdSeParse<SdKiCadFpLine,&SdKiCadFpLine::mStroke> },
      { "layer",  sdSeParse<SdKiCadFpLine,&SdKiCadFpLine::mLayer> }
    };

    bool isValid() const { return mStart.isValid() && mEnd.isValid() && mStroke.isValid() && mLayer.isValid(); }
  };



struct SdKiCadFpRect : public SdSeGroup<SdKiCadFpRect>
  {
    SdKiCadFpSdPoint mStart;
    SdKiCadFpSdPoint mEnd;
    SdKiCadStroke    mStroke;
    SdSeString       mLayer;
    SdKiCadFillType  mFillType;

    inline static SdSeTokenMap<SdKiCadFpRect> tokenMap = {
      { "start",  sdSeParse<SdKiCadFpRect,&SdKiCadFpRect::mStart> },
      { "end",    sdSeParse<SdKiCadFpRect,&SdKiCadFpRect::mEnd> },
      { "stroke", sdSeParse<SdKiCadFpRect,&SdKiCadFpRect::mStroke> },
      { "layer",  sdSeParse<SdKiCadFpRect,&SdKiCadFpRect::mLayer> },
      { "fill",   sdSeParse<SdKiCadFpRect,&SdKiCadFpRect::mFillType> }
    };
  };



struct SdKiCadFpCircle : public SdSeGroup<SdKiCadFpCircle>
  {
    SdKiCadFpSdPoint mCenter;
    SdKiCadFpSdPoint mEnd;
    SdKiCadStroke    mStroke;
    SdKiCadFillType  mFillType;
    SdSeString       mLayer;

    inline static SdSeTokenMap<SdKiCadFpCircle> tokenMap = {
      { "center", sdSeParse<SdKiCadFpCircle,&SdKiCadFpCircle::mCenter> },
      { "end",    sdSeParse<SdKiCadFpCircle,&SdKiCadFpCircle::mEnd> },
      { "stroke", sdSeParse<SdKiCadFpCircle,&SdKiCadFpCircle::mStroke> },
      { "fill",   sdSeParse<SdKiCadFpCircle,&SdKiCadFpCircle::mFillType> },
      { "layer",  sdSeParse<SdKiCadFpCircle,&SdKiCadFpCircle::mLayer> }
    };

    bool isValid() const { return mCenter.isValid() && mEnd.isValid() && mStroke.isValid() && mLayer.isValid(); }
  };



struct SdKiCadFpArc : public SdSeGroup<SdKiCadFpArc>
  {
    SdSePoint     mStart;
    SdSePoint     mMid;
    SdSePoint     mEnd;
    SdKiCadStroke mStroke;
    SdSeString    mLayer;

    inline static SdSeTokenMap<SdKiCadFpArc> tokenMap = {
      { "start",  sdSeParse<SdKiCadFpArc,&SdKiCadFpArc::mStart> },
      { "mid",    sdSeParse<SdKiCadFpArc,&SdKiCadFpArc::mMid> },
      { "end",    sdSeParse<SdKiCadFpArc,&SdKiCadFpArc::mEnd> },
      { "stroke", sdSeParse<SdKiCadFpArc,&SdKiCadFpArc::mStroke> },
      { "layer",  sdSeParse<SdKiCadFpArc,&SdKiCadFpArc::mLayer> }
    };

    bool isValid() const { return mStart.isValid() && mMid.isValid() && mEnd.isValid() && mStroke.isValid() && mLayer.isValid(); }
  };


/*





struct SdKiCadTextProp : public SdKiCadGrouped<SdKiCadTextProp>
  {
    SdKiCadSdPoint          mPos;
    SdKiCadString  mLayer;

  };

*/

/*
void kicadFootprint( SdScanerKiCad *scaner, SdProject *project );
void kicadSymbolLib( SdScanerKiCad *scaner, SdProject *project );
void kicadPropertyPart( SdScanerKiCad *scaner, SdPItemPart *part );
void kicadGraphLine( SdScanerKiCad *scaner, SdPItemPart *part );
void kicadGraphCircle( SdScanerKiCad *scaner, SdPItemPart *part );

void kicadFillPropLine( SdPropLine &prop, const SdKiCadStroke &stroke, const SdKiCadString &layer );
QString kicadLayerMap( const QString kicadLayerName );
*/

#endif // SDKICAD_H
