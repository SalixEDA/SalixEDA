#include "SdKiCadFootprint.h"
#include "objects/SdPItemPart.h"
#include "objects/SdProject.h"
#include "objects/SdGraphLinearLine.h"
#include "objects/SdGraphLinearArc.h"
#include "objects/SdGraphLinearCircle.h"
#include "objects/SdGraphLinearCircleFilled.h"
#include "objects/SdGraphLinearRect.h"
#include "objects/SdGraphLinearRectFilled.h"


SdKiCadFootprint::SdKiCadFootprint(SdProject *prj) :
  mDestProject(prj),
  mPart(nullptr)
  {

  }

void SdKiCadFootprint::prolog(SdScanerSe *)
  {
  //Create part
  mPart = new SdPItemPart();
  }

void SdKiCadFootprint::epilog(SdScanerSe *)
  {
  if( mPart != nullptr ) {
    //Set part name
    mPart->setTitle( mName, QString{} );
    mPart->setUnicalTitle( QString{} );

    //Insert part into project
    mDestProject->insertChild( mPart, mDestProject->getUndo() );

    mPart = nullptr;
    }
  }

void SdKiCadFootprint::fp_text(SdScanerSe *scaner)
  {
  SdKiCadFpText fpText;
  fpText.scan( scaner );
  }




void SdKiCadFootprint::fp_line(SdScanerSe *scaner)
  {
  SdKiCadFpLine fpLine;
  fpLine.scan( scaner );

  SdPropLine propLine;
  fillPropLine( propLine, fpLine.mStroke, fpLine.mLayer );

  SdPoint p1(fpLine.mStart.point());
  SdPoint p2(fpLine.mEnd.point());
  if( p1 != p2 ) {
    auto line = new SdGraphLinearLine( p1, p2, propLine );
    mPart->insertChild( line, nullptr );
    }
  }




void SdKiCadFootprint::fp_rect(SdScanerSe *scaner)
  {
  SdKiCadFpRect fpRect;
  fpRect.scan( scaner );

  SdPropLine propLine;
  fillPropLine( propLine, fpRect.mStroke, fpRect.mLayer );

  SdPoint p1(fpRect.mStart.point());
  SdPoint p2(fpRect.mEnd.point());
  if( p1 != p2 ) {
    SdGraph *graph;
    if( fpRect.mFillType.isValid() && fpRect.mFillType.selector() == SdKiCadFillType::typeSolid )
      graph = new SdGraphLinearRectFilled( p1, p2, propLine );
    else
      graph = new SdGraphLinearRect( p1, p2, propLine );
    mPart->insertChild( graph, nullptr );
    }
  }




void SdKiCadFootprint::fp_circle(SdScanerSe *scaner)
  {
  SdKiCadFpCircle fpCircle;
  fpCircle.scan( scaner );

  SdPropLine propLine;
  fillPropLine( propLine, fpCircle.mStroke, fpCircle.mLayer );

  SdPoint center( fpCircle.mCenter.point() );
  SdPoint end( fpCircle.mEnd.point() );

  if( center != end ) {
    int radius = center.getDistanceInt( end );
    SdGraph *graph;
    if( fpCircle.mFillType.isValid() && fpCircle.mFillType.selector() == SdKiCadFillType::typeSolid )
      graph = new SdGraphLinearCircleFilled( center, radius, propLine );
    else
      graph = new SdGraphLinearCircle( center, radius, propLine );
    mPart->insertChild( graph, nullptr );
    }
  }



void SdKiCadFootprint::fp_arc(SdScanerSe *scaner)
  {
  SdKiCadFpArc fpArc;
  fpArc.scan( scaner );

  SdPropLine propLine;
  fillPropLine( propLine, fpArc.mStroke, fpArc.mLayer );

  QPointF p1(fpArc.mStart.point());
  QPointF p2(fpArc.mMid.point());
  QPointF p3(fpArc.mEnd.point());

  if( p1 == p3 )
    return;

  //Calculate center of circle passing through p1, p2, p3
  const double ax = p1.x();
  const double ay = p1.y();
  const double bx = p2.x();
  const double by = p2.y();
  const double cx = p3.x();
  const double cy = p3.y();

  const double d = 2.0 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
  if( qAbs(d) < 1e-6 )
    return;

  const double a2 = ax * ax + ay * ay;
  const double b2 = bx * bx + by * by;
  const double c2 = cx * cx + cy * cy;

  const double ux = (a2 * (by - cy) + b2 * (cy - ay) + c2 * (ay - by)) / d;
  const double uy = (a2 * (cx - bx) + b2 * (ax - cx) + c2 * (bx - ax)) / d;

  SdPoint center( qRound(ux), qRound(uy) );

  auto arc = new SdGraphLinearArc( center, fpArc.mStart.qPoint(SdKiCadFpScale),
                                   fpArc.mMid.qPoint(SdKiCadFpScale), fpArc.mEnd.qPoint(SdKiCadFpScale), propLine );
  mPart->insertChild( arc, nullptr );
  }



void SdKiCadFootprint::fp_poly(SdScanerSe *scaner)
  {

  }

void SdKiCadFootprint::fp_pad(SdScanerSe *scaner)
  {

  }




void SdKiCadFootprint::fillPropLine(SdPropLine &prop, const SdKiCadStroke &stroke, const SdSeString &layer)
  {
  if( stroke.mWidth.isValid() )
    prop.mWidth = stroke.mWidth.value();
  else
    prop.mWidth = 0;

  if( stroke.mType.isValid() ) {
    switch( stroke.mType.selector() ) {
      case SdKiCadStrokeType::typeDotted : prop.mType = dltDotted; break;
      case SdKiCadStrokeType::typeDashed : prop.mType = dltDashed; break;
      default :
        prop.mType = dltSolid;
      }
    }
  else
    prop.mType = dltSolid;

  prop.mLayer.set( layerMap(layer.string()) );
  }











struct SdKiCadLayerMapDescription
  {
    const char *mKiCadId;
    const char *mLayerId;
  };


static SdKiCadLayerMapDescription kicadLayerMapDescriptionForPads[] = {
  // ================================================================
  // Pad-specific layers (специальные случаи для падов)
  // ================================================================
  { "F.Cu", LID0_PAD LID1_TOP },              // Пады на верхнем слое
  { "B.Cu", LID0_PAD LID1_BOT },              // Пады на нижнем слое
  { "In1.Cu", LID0_PAD LID1_INT02 },          // Пады на внутреннем слое 1
  { "In2.Cu", LID0_PAD LID1_INT03 },          // ... и так далее до In28.Cu
  { "In3.Cu", LID0_PAD LID1_INT04 },
  { "In4.Cu", LID0_PAD LID1_INT05 },
  { "In5.Cu", LID0_PAD LID1_INT06 },
  { "In6.Cu", LID0_PAD LID1_INT07 },
  { "In7.Cu", LID0_PAD LID1_INT08 },
  { "In8.Cu", LID0_PAD LID1_INT09 },
  { "In9.Cu", LID0_PAD LID1_INT10 },
  { "In10.Cu", LID0_PAD LID1_INT11 },
  { "In11.Cu", LID0_PAD LID1_INT12 },
  { "In12.Cu", LID0_PAD LID1_INT13 },
  { "In13.Cu", LID0_PAD LID1_INT14 },
  { "In14.Cu", LID0_PAD LID1_INT15 },
  { "In15.Cu", LID0_PAD LID1_INT16 },
  { "In16.Cu", LID0_PAD LID1_INT17 },
  { "In17.Cu", LID0_PAD LID1_INT18 },
  { "In18.Cu", LID0_PAD LID1_INT19 },
  { "In19.Cu", LID0_PAD LID1_INT20 },
  { "In20.Cu", LID0_PAD LID1_INT21 },
  { "In21.Cu", LID0_PAD LID1_INT22 },
  { "In22.Cu", LID0_PAD LID1_INT23 },
  { "In23.Cu", LID0_PAD LID1_INT24 },
  { "In24.Cu", LID0_PAD LID1_INT25 },
  { "In25.Cu", LID0_PAD LID1_INT26 },
  { "In26.Cu", LID0_PAD LID1_INT27 },
  { "In27.Cu", LID0_PAD LID1_INT28 },
  { "In28.Cu", LID0_PAD LID1_INT29 },

  // Завершающий элемент
  { nullptr, nullptr }
};



static SdKiCadLayerMapDescription kicadLayerMapDescriptionForPolygon[] = {
  // ================================================================
  // Polygon-specific layers (специальные случаи для полигонов)
  // ================================================================
  { "F.Cu", LID0_POLYGON LID1_TOP },         // Полигоны на верхнем слое
  { "B.Cu", LID0_POLYGON LID1_BOT },         // Полигоны на нижнем слое
  { "In1.Cu", LID0_POLYGON LID1_INT02 },     // Полигоны на внутренних слоях
  { "In2.Cu", LID0_POLYGON LID1_INT03 },
  { "In3.Cu", LID0_POLYGON LID1_INT04 },
  { "In4.Cu", LID0_POLYGON LID1_INT05 },
  { "In5.Cu", LID0_POLYGON LID1_INT06 },
  { "In6.Cu", LID0_POLYGON LID1_INT07 },
  { "In7.Cu", LID0_POLYGON LID1_INT08 },
  { "In8.Cu", LID0_POLYGON LID1_INT09 },
  { "In9.Cu", LID0_POLYGON LID1_INT10 },
  { "In10.Cu", LID0_POLYGON LID1_INT11 },
  { "In11.Cu", LID0_POLYGON LID1_INT12 },
  { "In12.Cu", LID0_POLYGON LID1_INT13 },
  { "In13.Cu", LID0_POLYGON LID1_INT14 },
  { "In14.Cu", LID0_POLYGON LID1_INT15 },
  { "In15.Cu", LID0_POLYGON LID1_INT16 },
  { "In16.Cu", LID0_POLYGON LID1_INT17 },
  { "In17.Cu", LID0_POLYGON LID1_INT18 },
  { "In18.Cu", LID0_POLYGON LID1_INT19 },
  { "In19.Cu", LID0_POLYGON LID1_INT20 },
  { "In20.Cu", LID0_POLYGON LID1_INT21 },
  { "In21.Cu", LID0_POLYGON LID1_INT22 },
  { "In22.Cu", LID0_POLYGON LID1_INT23 },
  { "In23.Cu", LID0_POLYGON LID1_INT24 },
  { "In24.Cu", LID0_POLYGON LID1_INT25 },
  { "In25.Cu", LID0_POLYGON LID1_INT26 },
  { "In26.Cu", LID0_POLYGON LID1_INT27 },
  { "In27.Cu", LID0_POLYGON LID1_INT28 },
  { "In28.Cu", LID0_POLYGON LID1_INT29 },


  // Завершающий элемент
  { nullptr, nullptr }
};




QString SdKiCadFootprint::layerMap(const QString &kiCadLayer)
  {
  static QMap<QString,QString> mLayerMap;
  if( mLayerMap.isEmpty() ) {
    static SdKiCadLayerMapDescription kicadLayerMapDescription[] = {
      // ================================================================
      // Copper layers (Медные слои)
      // ================================================================
      { "F.Cu", LID0_TRACE LID1_TOP },           // Верхний медный слой - трассы
      { "B.Cu", LID0_TRACE LID1_BOT },           // Нижний медный слой - трассы
      { "In1.Cu", LID0_TRACE LID1_INT02 },       // Внутренний слой 1 - трассы
      { "In2.Cu", LID0_TRACE LID1_INT03 },       // Внутренний слой 2 - трассы
      { "In3.Cu", LID0_TRACE LID1_INT04 },       // Внутренний слой 3 - трассы
      { "In4.Cu", LID0_TRACE LID1_INT05 },       // Внутренний слой 4 - трассы
      { "In5.Cu", LID0_TRACE LID1_INT06 },       // Внутренний слой 5 - трассы
      { "In6.Cu", LID0_TRACE LID1_INT07 },       // Внутренний слой 6 - трассы
      { "In7.Cu", LID0_TRACE LID1_INT08 },       // Внутренний слой 7 - трассы
      { "In8.Cu", LID0_TRACE LID1_INT09 },       // Внутренний слой 8 - трассы
      { "In9.Cu", LID0_TRACE LID1_INT10 },       // Внутренний слой 9 - трассы
      { "In10.Cu", LID0_TRACE LID1_INT11 },      // Внутренний слой 10 - трассы
      { "In11.Cu", LID0_TRACE LID1_INT12 },      // Внутренний слой 11 - трассы
      { "In12.Cu", LID0_TRACE LID1_INT13 },      // Внутренний слой 12 - трассы
      { "In13.Cu", LID0_TRACE LID1_INT14 },      // Внутренний слой 13 - трассы
      { "In14.Cu", LID0_TRACE LID1_INT15 },      // Внутренний слой 14 - трассы
      { "In15.Cu", LID0_TRACE LID1_INT16 },      // Внутренний слой 15 - трассы
      { "In16.Cu", LID0_TRACE LID1_INT17 },      // Внутренний слой 16 - трассы
      { "In17.Cu", LID0_TRACE LID1_INT18 },      // Внутренний слой 17 - трассы
      { "In18.Cu", LID0_TRACE LID1_INT19 },      // Внутренний слой 18 - трассы
      { "In19.Cu", LID0_TRACE LID1_INT20 },      // Внутренний слой 19 - трассы
      { "In20.Cu", LID0_TRACE LID1_INT21 },      // Внутренний слой 20 - трассы
      { "In21.Cu", LID0_TRACE LID1_INT22 },      // Внутренний слой 21 - трассы
      { "In22.Cu", LID0_TRACE LID1_INT23 },      // Внутренний слой 22 - трассы
      { "In23.Cu", LID0_TRACE LID1_INT24 },      // Внутренний слой 23 - трассы
      { "In24.Cu", LID0_TRACE LID1_INT25 },      // Внутренний слой 24 - трассы
      { "In25.Cu", LID0_TRACE LID1_INT26 },      // Внутренний слой 25 - трассы
      { "In26.Cu", LID0_TRACE LID1_INT27 },      // Внутренний слой 26 - трассы
      { "In27.Cu", LID0_TRACE LID1_INT28 },      // Внутренний слой 27 - трассы
      { "In28.Cu", LID0_TRACE LID1_INT29 },      // Внутренний слой 28 - трассы (макс в вашей системе)
      { "In29.Cu", nullptr },                    // Не используется (ограничение 30 слоев)
      { "In30.Cu", nullptr },                   // Не используется (ограничение 30 слоев)

      // ================================================================
      // Solder Mask / Паяльная маска
      // ================================================================
      { "F.Mask", LID0_SOLDER_MASK LID1_TOP },   // Верхняя паяльная маска
      { "B.Mask", LID0_SOLDER_MASK LID1_BOT },   // Нижняя паяльная маска

      // Внутренние слои не имеют маски в KiCad

      // ================================================================
      // Paste / Паяльная паста (трафарет)
      // ================================================================
      { "F.Paste", LID0_STENCIL LID1_TOP },      // Верхний трафарет
      { "B.Paste", LID0_STENCIL LID1_BOT },      // Нижний трафарет

      // ================================================================
      // Silkscreen / Шелкография
      // ================================================================
      { "F.SilkS", LID0_SILK LID1_TOP },         // Верхняя шелкография
      { "B.SilkS", LID0_SILK LID1_BOT },         // Нижняя шелкография

      // ================================================================
      // Courtyard / Зона компонента
      // ================================================================
      { "F.CrtYd", LID0_COURTYARD LID1_TOP },    // Верхняя зона компонента
      { "B.CrtYd", LID0_COURTYARD LID1_BOT },    // Нижняя зона компонента

      // ================================================================
      // Fabrication / Сборочный чертеж
      // ================================================================
      { "F.Fab", LID0_COMPONENT LID1_TOP },      // Верхний сборочный чертеж (корпус)
      { "B.Fab", LID0_COMPONENT LID1_BOT },      // Нижний сборочный чертеж (корпус)

      // ================================================================
      // Adhesive / Клей
      // ================================================================
      { "F.Adhes", LID0_ADHESIV LID1_TOP },      // Верхний клей
      { "B.Adhes", LID0_ADHESIV LID1_BOT },      // Нижний клей

      // ================================================================
      // Keep-Out / Запрет трассировки
      // ================================================================
      { "F.KeepOut", LID0_EXCLUSION LID1_TOP },  // Верхний запрет трассировки
      { "B.KeepOut", LID0_EXCLUSION LID1_BOT },  // Нижний запрет трассировки

      // Внутренние слои Keep-Out
      { "In1.KeepOut", LID0_EXCLUSION LID1_INT02 },
      { "In2.KeepOut", LID0_EXCLUSION LID1_INT03 },
      { "In3.KeepOut", LID0_EXCLUSION LID1_INT04 },
      { "In4.KeepOut", LID0_EXCLUSION LID1_INT05 },
      { "In5.KeepOut", LID0_EXCLUSION LID1_INT06 },
      { "In6.KeepOut", LID0_EXCLUSION LID1_INT07 },
      { "In7.KeepOut", LID0_EXCLUSION LID1_INT08 },
      { "In8.KeepOut", LID0_EXCLUSION LID1_INT09 },
      { "In9.KeepOut", LID0_EXCLUSION LID1_INT10 },
      { "In10.KeepOut", LID0_EXCLUSION LID1_INT11 },
      { "In11.KeepOut", LID0_EXCLUSION LID1_INT12 },
      { "In12.KeepOut", LID0_EXCLUSION LID1_INT13 },
      { "In13.KeepOut", LID0_EXCLUSION LID1_INT14 },
      { "In14.KeepOut", LID0_EXCLUSION LID1_INT15 },
      { "In15.KeepOut", LID0_EXCLUSION LID1_INT16 },
      { "In16.KeepOut", LID0_EXCLUSION LID1_INT17 },
      { "In17.KeepOut", LID0_EXCLUSION LID1_INT18 },
      { "In18.KeepOut", LID0_EXCLUSION LID1_INT19 },
      { "In19.KeepOut", LID0_EXCLUSION LID1_INT20 },
      { "In20.KeepOut", LID0_EXCLUSION LID1_INT21 },
      { "In21.KeepOut", LID0_EXCLUSION LID1_INT22 },
      { "In22.KeepOut", LID0_EXCLUSION LID1_INT23 },
      { "In23.KeepOut", LID0_EXCLUSION LID1_INT24 },
      { "In24.KeepOut", LID0_EXCLUSION LID1_INT25 },
      { "In25.KeepOut", LID0_EXCLUSION LID1_INT26 },
      { "In26.KeepOut", LID0_EXCLUSION LID1_INT27 },
      { "In27.KeepOut", LID0_EXCLUSION LID1_INT28 },
      { "In28.KeepOut", LID0_EXCLUSION LID1_INT29 },

      // ================================================================
      // Edge Cuts / Контур платы
      // ================================================================
      { "Edge.Cuts", LID0_GUIDE LID1_TOP LID1_BOT },  // Контур платы (специальный случай)

      // ================================================================
      // Margin / Поле
      // ================================================================
      { "Margin", LID0_GUIDE LID1_TOP LID1_BOT },     // Электрическая граница

      // ================================================================
      // User layers / Пользовательские слои
      // ================================================================
      { "Dwgs.User", LID0_DIM LID1_TOP LID1_BOT },    // Чертежи пользователя
      { "Cmts.User", LID0_IDENT LID1_TOP LID1_BOT },  // Комментарии
      { "Eco1.User", LID0_GUIDE LID1_TOP LID1_BOT },  // ECO 1
      { "Eco2.User", LID0_GUIDE LID1_TOP LID1_BOT },  // ECO 2


      // ================================================================
      // Via-specific layers (переходные отверстия)
      // ================================================================
      { "Via", LID0_WIRE LID1_TOP LID1_BOT },  // Сквозные переходные отверстия
      { "Via.In1", LID0_HOLE LID1_INT02 },     // Отверстие между TOP и INT00
      { "Via.In2", LID0_HOLE LID1_INT03 },     // и так далее...
      { "Via.In3", LID0_HOLE LID1_INT04 },
      { "Via.In4", LID0_HOLE LID1_INT05 },
      { "Via.In5", LID0_HOLE LID1_INT06 },
      { "Via.In6", LID0_HOLE LID1_INT07 },
      { "Via.In7", LID0_HOLE LID1_INT08 },
      { "Via.In8", LID0_HOLE LID1_INT09 },
      { "Via.In9", LID0_HOLE LID1_INT10 },
      { "Via.In10", LID0_HOLE LID1_INT11 },
      { "Via.In11", LID0_HOLE LID1_INT12 },
      { "Via.In12", LID0_HOLE LID1_INT13 },
      { "Via.In13", LID0_HOLE LID1_INT14 },
      { "Via.In14", LID0_HOLE LID1_INT15 },
      { "Via.In15", LID0_HOLE LID1_INT16 },
      { "Via.In16", LID0_HOLE LID1_INT17 },
      { "Via.In17", LID0_HOLE LID1_INT18 },
      { "Via.In18", LID0_HOLE LID1_INT19 },
      { "Via.In19", LID0_HOLE LID1_INT20 },
      { "Via.In20", LID0_HOLE LID1_INT21 },
      { "Via.In21", LID0_HOLE LID1_INT22 },
      { "Via.In22", LID0_HOLE LID1_INT23 },
      { "Via.In23", LID0_HOLE LID1_INT24 },
      { "Via.In24", LID0_HOLE LID1_INT25 },
      { "Via.In25", LID0_HOLE LID1_INT26 },
      { "Via.In26", LID0_HOLE LID1_INT26 },
      { "Via.In27", LID0_HOLE LID1_INT27 },
      { "Via.In28", LID0_HOLE LID1_INT28 },

      // Завершающий элемент
      { nullptr, nullptr }
    };

    //Fill layer map
    for( int i = 0; kicadLayerMapDescription[i].mKiCadId != nullptr; i++ )
      mLayerMap.insert( kicadLayerMapDescription[i].mKiCadId, kicadLayerMapDescription[i].mLayerId );
    }
  return mLayerMap.value( kiCadLayer, LID0_INVISIBLE );
  }
