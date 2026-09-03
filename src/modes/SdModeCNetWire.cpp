/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Mode for enter wire in sheet
*/
#include "SdModeCNetWire.h"
#include "objects/SdGraph.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdEnvir.h"
#include "objects/SdProject.h"
#include "objects/SdGraphSymImp.h"
#include "objects/SdGraphNetWire.h"
#include "windows/SdPropBarWire.h"
#include "windows/SdWCommand.h"
#include "windows/SdDNetUnion.h"
#include "windows/SdWEditorGraph.h"
#include <QMessageBox>
#include <QDebug>

SdModeCNetWire::SdModeCNetWire(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {

  }




void SdModeCNetWire::drawStatic(SdContext *ctx)
  {
  //All objects draw normally except net with netName.
  mObject->forEach( dctAll, [this,ctx] (SdObject *obj) -> bool {
    SdGraphNet *net = dynamic_cast<SdGraphNet*>( obj );
    if( net != nullptr ) {
      if( !mShowNet || mNetName != net->getNetName() )
        net->draw( ctx );
      }
    else {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph != nullptr )
        graph->draw( ctx );
      }
    return true;
    });

  //Draw if net present
  if( !mNetName.isEmpty() && mShowNet ) {
    ctx->setOverColor( SdEnvir::instance()->getSysColor(scEnter) );
    mObject->forEach( dctNetWire | dctNetName | dctNetParam, [this,ctx] (SdObject *obj) -> bool {
      SdGraphNet *net = dynamic_cast<SdGraphNet*>( obj );
      if( net != nullptr && mNetName == net->getNetName() )
        net->draw( ctx );
      return true;
      });
    ctx->resetOverColor();
    }
  }




void SdModeCNetWire::drawDynamic(SdContext *ctx)
  {
  if( getStep() ) {
    //Draw entering segment with possible vertex
    ctx->setOverColor( SdEnvir::instance()->getSysColor(scEnter) );
    if( mFirst != mMiddle ) ctx->line( mFirst, mMiddle, sdGlobalProp->mWireProp );
    if( mMiddle != mPrevMove ) ctx->line( mMiddle, mPrevMove, sdGlobalProp->mWireProp );
    ctx->resetOverColor();
    }
  if( SdEnvir::instance()->mIsWireSmart && mSmartSour != mSmartDest ) {
    //Draw smart variant
    ctx->setOverColor( SdEnvir::instance()->getSysColor(scSmart) );
    //Draw source stringer if present
    if( mSmartSour != mSmartSourStr )   ctx->line( mSmartSour, mSmartSourStr, sdGlobalProp->mWireProp );
    //Draw first intermediate vertex
    if( mSmartSourStr != mSmartInter0 ) ctx->line( mSmartSourStr, mSmartInter0, sdGlobalProp->mWireProp );
    //Draw second intermediate vertex
    if( mSmartInter0 != mSmartInter1 )  ctx->line( mSmartInter0, mSmartInter1, sdGlobalProp->mWireProp );
    //Draw destignation vertex
    if( mSmartInter1 != mSmartDestStr ) ctx->line( mSmartInter1, mSmartDestStr, sdGlobalProp->mWireProp );
    //Draw destignation stringer if present
    if( mSmartDestStr != mSmartDest )   ctx->line( mSmartDestStr, mSmartDest, sdGlobalProp->mWireProp );
    ctx->smartPoint( mSmartDest, snapNetPoint );
    ctx->resetOverColor();
    }
  }




int SdModeCNetWire::getPropBarId() const
  {
  return SdPropBarWire::mBarId;
  }



void SdModeCNetWire::propGetFromBar()
  {
  if( SdPropBarWirePtr bar{} ) {
    SdPvMulty<SdPvString> pvWireName;
    QString wireName;
    bar->getPropWire( sdGlobalProp->mWireProp, &(sdGlobalProp->mWireEnterType), pvWireName );
    Q_ASSERT( pvWireName.isSingle() );
    wireName = pvWireName.value().string();
    if( getStep() ) {
      //If wire enter in process then perhaps net union. Detect it
      if( mNetName != wireName ) {
        //Entered new wire name
        if( getSheet()->isNetPresent(wireName) ) {
          if( getSheet()->isNetPresent(mNetName) ) {
            //Both nets present: with new name and with old name
            if( okUnion(wireName) ) {
              auto oldNetName = mNetName;
              getSheet()->netRename( mNetName, wireName, mUndo );
              //Question: rename net in other sheets and rename if yes
              renameOtherSheets( oldNetName, wireName );
              }
            }
          }
        else {
          if( getSheet()->isNetPresent(mNetName) ) {
            auto oldNetName = mNetName;
            //New net is not defined yet. Rename old net
            getSheet()->netRename( mNetName, wireName, mUndo );
            //Question: rename net in other sheets and rename if yes
            renameOtherSheets( oldNetName, wireName );
            }
          }
        mNetName = wireName;
        }
      }
    else {
      mNetName = wireName;
      }
    mEditor->setFocus();
    update();
    }
  }





void SdModeCNetWire::propSetToBar()
  {
  if( SdPropBarWirePtr bar{} ) {
    SdPvMulty<SdPvString> netName;
    netName.reset( SdPvString(mNetName) );
    bar->setPropWire( sdGlobalProp->mWireProp, mEditor->getPPM(), sdGlobalProp->mWireEnterType, netName );
    }
  }




void SdModeCNetWire::enterPoint( SdPoint enter )
  {
  if( getStep() ) {
    mPrevMove = calcMiddlePoint( mFirst, enter, sdGlobalProp->mWireEnterType );
    if( mPrevMove == mFirst ) mPrevMove = enter;
    mShowNet = false;
    setDirtyCashe();
    //Append segment
    if( testNextPoint( mPrevMove ) && mFirst != mPrevMove ) {
      mUndo->begin( QObject::tr("Insert wire segment"), mObject, false );
      mObject->insertChild( new SdGraphNetWire( mFirst, mPrevMove, mNetName, sdGlobalProp->mWireProp ), mUndo );
      mFirst = mPrevMove;
      setDirty();
      }
    mPrevMove = enter;
    mMiddle = calcMiddlePoint( mFirst, mPrevMove, sdGlobalProp->mWireEnterType );
    calcSmartPoint();
    }
  else {
    mFirst = enter;
    mPrevMove = mFirst;
    if( testFirstPoint( mFirst ) ) setStep(sNextPoint);
    //Reset smart
    mMiddle = enter;
    mSmartSour = enter;
    mSmartDest = enter;
    }
  update();
  }



void SdModeCNetWire::cancelPoint(SdPoint)
  {
  update();
  if( getStep() ) nextNet();
  else cancelMode();
  }



void SdModeCNetWire::movePoint( SdPoint p )
  {
  if( getStep() ) {
    mPrevMove = p;
    mMiddle = calcMiddlePoint( mFirst, mPrevMove, sdGlobalProp->mWireEnterType );
    //For smart point we request is it sym imp
    mSmartSour = mFirst;
    int state = 0;
    SdGraphSdp graph( getSheet()->behindPoint( dctSymImp, mFirst, &state ) );
    mSmartSourStr = getStringer( mSmartSour.x(), mSmartSour.y(), graph.ptr() );
    calcSmartPoint();
    }
  else {
    mPrevMove = p;
    calcFirstSmart();
    }
  update();
  }




SdPoint SdModeCNetWire::enterPrev()
  {
  mUndo->begin( QObject::tr("Insert smart net"), mObject, false );
  if( getStep() )
    //Net end variant
    insertSmartNet();
  else {
    //Full routing net variant
    if( testFirstPoint( mSmartSour ) )
      insertSmartNet();
    }
  nextNet();
  setDirty();
  calcFirstSmart();
  mFirst = mPrevMove;
  mMiddle = mFirst;
  update();
  return mFirst;
  }






QString SdModeCNetWire::getStepHelp() const
  {
  return getStep() == sNextPoint ? QObject::tr("Enter next point of wire polyline") : QObject::tr("Enter first wire point");
  }




QString SdModeCNetWire::getModeThema() const
  {
  return QString( MODE_HELP "ModeCNetWire.htm" );
  }




QString SdModeCNetWire::getStepThema() const
  {
  return getStep() == sNextPoint ? QString( MODE_HELP "ModeCNetWire.htm#nextPoint" ) : QString( MODE_HELP "ModeCNetWire.htm#firstPoint" );
  }




int SdModeCNetWire::getCursor() const
  {
  return CUR_WIRE;
  }




int SdModeCNetWire::getIndex() const
  {
  return MD_WIRE;
  }




SdModeCNetWire::RenumResult SdModeCNetWire::getUnionResult(const QString firstWireName, const QString secondWireName)
  {
  return static_cast<SdModeCNetWire::RenumResult>( SdDNetUnion( firstWireName, secondWireName, mEditor ).exec() );
  }




bool SdModeCNetWire::okUnion(const QString newName)
  {
  return QMessageBox::question( mEditor, QObject::tr("Warning!"), QObject::tr("Union net '%1' and net '%2'?").arg(mNetName, newName) ) == QMessageBox::Yes;
  }



void SdModeCNetWire::renameOtherSheets( const QString &oldNetName, const QString newNetName )
  {
  //In first we find nets with mNetName in other sheets
  bool oldNetPresent = false;
  SdProject *prj = getSheet()->getProject();
  Q_ASSERT(prj != nullptr);
  prj->forEach( dctSheet, [oldNetName, &oldNetPresent] (SdObject *obj) -> bool {
    if( SdPItemSheetSdp sheet{obj} )
      oldNetPresent = sheet->isNetPresent( oldNetName );
    return !oldNetPresent;
    });

  //We show dialog only if there is oldNetName in other sheets
  if( oldNetPresent ) {
    if( QMessageBox::question( mEditor, QObject::tr("Query"), QObject::tr("Rename nets in all other sheets?")) == QMessageBox::Yes ) {
      //User answer "Yes", so for each sheet in project execute renaming
      prj->forEach( dctSheet, [this,oldNetName,newNetName] (SdObject *obj) -> bool {
        if( SdPItemSheetSdp sheet{obj} )
          sheet->netRename( oldNetName, newNetName, mUndo );
        return true;
        });
      }
    }
  }





bool SdModeCNetWire::testFirstPoint(SdPoint p)
  {
  QString tmpName;
  if( getSheet()->getNetFromPoint( p, tmpName ) ) {
    if( mNetName.startsWith( defNetNamePrefix ) )
      //Our net has default name, assign it found net name
      mNetName = tmpName;
    else {
      if( tmpName.startsWith( defNetNamePrefix ) )
        //Net has default name, override it with our existing net (rename)
        renameNet( tmpName, mNetName );
      else {
        //Nets have not default names. Query user to deсision what name assign
        switch( getUnionResult( tmpName, mNetName ) ) {
          case renFirst  : mNetName = tmpName; break;             //Lets first
          case renSecond : renameNet( tmpName, mNetName ); break; //Lets second
          default : return false;
          }
        }
      }
    }
  propSetToBar();
  mShowNet = getSheet()->isNetPresent( mNetName );
  setDirtyCashe();
  return true;
  }




bool SdModeCNetWire::testNextPoint(SdPoint p)
  {
  QString tmpName;
  if( getSheet()->getNetFromPoint( p, tmpName ) && tmpName != mNetName ) {
    if( okUnion(tmpName) ) {
      //Net union
      if( mNetName.startsWith( defNetNamePrefix ) ) renameNet( mNetName, tmpName );
      else if( tmpName.startsWith( defNetNamePrefix ) ) renameNet( tmpName, mNetName );
      else {
        //Nets have not default names. Query user to deсision what name assign
        switch( getUnionResult( mNetName, tmpName ) ) {
          case renFirst  : renameNet( tmpName, mNetName ); break; //Lets first
          case renSecond : renameNet( mNetName, tmpName ); break; //Lets second
          default : return false;
          }
        }
      }
    else return false;
    mShowNet = getSheet()->isNetPresent( mNetName );
    setDirtyCashe();
    }
  propSetToBar();
  return true;
  }




void SdModeCNetWire::renameNet(const QString sour, const QString dest)
  {
  getSheet()->netRename( sour, dest, mUndo );
  mNetName = dest;
  }






void SdModeCNetWire::calcFirstSmart()
  {
  //Prepare snap query
  SdSnapInfo snap;
  snap.mSour     = mPrevMove;
  snap.mSnapMask = snapNearestNet | snapNearestPin;
  snap.mExclude  = mPrevMove;
  snap.mDest     = mPrevMove;
  //Scan snap point
  snap.scan( getSheet() );
  //Assign founded snap point
  mSmartSour = snap.mDest;
  //Calculate stringer for source point if need
  mSmartSourStr = getStringer( mSmartSour.x(), mSmartSour.y(), snap.mGraph );
  //Calculate destignation snap
  calcSmartPoint();
  }









//static SdPoint pointToFar( SdPoint sour, SdPoint dest ) {
//  if( sour.x() == dest.x() ) {
//    if( sour.y() > dest.y() ) sour.ry()--;
//    else sour.ry()++;
//    }
//  else if( sour.y() == dest.y() ) {
//    if( sour.x() > dest.x() ) sour.rx()--;
//    else sour.rx()++;
//    }
//  return sour;
//  }





void SdModeCNetWire::calcSmartPoint()
  {
  SdSnapInfo snap;
  snap.mSour     = mPrevMove;
  snap.mSnapMask = snapNearestPin | snapExcludeExcl;
  snap.mExclude  = mSmartSour;
  snap.scan( getSheet(), dctSymImp );
  bool noResult = snap.mGraph == nullptr;

  //qDebug() << "calcSmartPoint" << noResult << snap.mDest;
  if( noResult ) {
    mSmartDest = mSmartSour;
    }
  else {
    SdRect over;
    over = snap.mGraph->getOverRect();

    mSmartDest = snap.mDest;
    //Calculate stringer
    //Stringer must have opposite direction with component body

    mSmartDestStr = getStringer( mSmartDest.x(), mSmartDest.y(), snap.mGraph );

    //Simple vertex
    mSmartInter0 = get90( mSmartSourStr, mSmartDestStr );
    mSmartInter1 = mSmartInter0;

    //Remove fail stringer
    if( mSmartInter0 == mSmartDest )
      mSmartDestStr = mSmartDest;

    }
  }




void SdModeCNetWire::nextNet()
  {
  setStep(sFirstPoint);
  mNetName = getSheet()->getProject()->getUnusedNetName();
  propSetToBar();
  mShowNet = false;
  setDirtyCashe();
  }








SdPItemSheet *SdModeCNetWire::getSheet()
  {
  return dynamic_cast<SdPItemSheet*>(mObject);
  }




//Insert prepared smart path net if present
void SdModeCNetWire::insertSmartNet()
  {
  //Perform insertion only if source and destignation points are not equals
  if( mSmartSour != mSmartDest ) {
    //Insert source stringer if present
    if( mSmartSour != mSmartSourStr )
      getSheet()->insertChild( new SdGraphNetWire( mSmartSour, mSmartSourStr, mNetName, sdGlobalProp->mWireProp ), mUndo );

    //Insert first intermediate vertex
    if( mSmartSourStr != mSmartInter0 )
      getSheet()->insertChild( new SdGraphNetWire( mSmartSourStr, mSmartInter0, mNetName, sdGlobalProp->mWireProp ), mUndo );

    //Insert second intermediate vertex
    if( mSmartInter0 != mSmartInter1 )
      getSheet()->insertChild( new SdGraphNetWire( mSmartInter0, mSmartInter1, mNetName, sdGlobalProp->mWireProp ), mUndo );

    //Insert destignation vertex
    if( mSmartInter1 != mSmartDestStr )
      getSheet()->insertChild( new SdGraphNetWire( mSmartInter1, mSmartDestStr, mNetName, sdGlobalProp->mWireProp ), mUndo );

    //Insert destignation stringer if present
    if( mSmartDestStr != mSmartDest )
      getSheet()->insertChild( new SdGraphNetWire( mSmartDestStr, mSmartDest, mNetName, sdGlobalProp->mWireProp ), mUndo );
    }
  }




//Calculate stringer only for sym imp
SdPoint SdModeCNetWire::getStringer(int x, int y, SdGraph *graph)
  {
  if( graph != nullptr && graph->getClass() == dctSymImp ) {
    int stringer = getGrid().x();


    SdRect over = graph->getOverRect();

    //Distance to left side of body
    int left = abs( over.getLeft() - x );
    //Distance to right side of body
    int right = abs( over.getRight() - x );
    //Minimum distance with horizontal direction
    int horz = qMin(left,right);

    //Distance to top side of bidy
    int top = abs( over.getTop() - y );
    //Distance to bottom side of body
    int bot = abs( over.getBottom() - y );
    //Minimum distance with vertical direction
    int vert = qMin( top, bot );

    //qDebug() << "stringer" << over.getTopLeft() << over.getBottomRight() << x << y;
    if( horz < vert ) {
      //Stringer is horizontal
      if( left < right )
        //Left direction
        x -= stringer;
      else
        //Right direction
        x += stringer;
      }
    else {
      //Stringer is vertical
      if( top < bot )
        //Top direction
        y += stringer;
      else
        //Bottom direction
        y -= stringer;
      }
    }
  return SdPoint(x,y);
  }




void SdModeCNetWire::activate()
  {
  nextNet();
  }



//Extended algorithm for smart route
#if 0
#include <QPoint>
#include <QRect>
#include <QVector>
#include <QLine>
#include <cmath>
#include <algorithm>
#include <climits>

class OrthogonalRouter {
public:
    // Функция округления к ближайшему шагу сетки
    static int snap(int value, int gridStep) {
        if (gridStep <= 0) return value;
        return std::round(static_cast<double>(value) / gridStep) * gridStep;
    }

    // Проверка: пересекает ли отрезок прямоугольник (с учетом исключения для нулевой толщины)
    static bool intersectsRect(const QPoint& p1, const QPoint& p2, const QRect& rect) {
        if (rect.isNull() || !rect.isValid()) return false;

        // Если это линия-точка или вырожденный прямоугольник
        bool isZeroWidth = (rect.width() == 0);
        bool isZeroHeight = (rect.height() == 0);

        // Определяем границы отрезка
        int minX = std::min(p1.x(), p2.x());
        int maxX = std::max(p1.x(), p2.x());
        int minY = std::min(p1.y(), p2.y());
        int maxY = std::max(p1.y(), p2.y());

        // Проверка прохода ВДОЛЬ ребер (запрещено по условию)
        // Если отрезок горизонтальный и лежит точно на верхней или нижней грани
        if (p1.y() == p2.y() && !isZeroHeight) {
            if ((p1.y() == rect.top() || p1.y() == rect.bottom()) && (maxX > rect.left() && minX < rect.right())) {
                return true;
            }
        }
        // Если отрезок вертикальный и лежит точно на левой или правой грани
        if (p1.x() == p2.x() && !isZeroWidth) {
            if ((p1.x() == rect.left() || p1.x() == rect.right()) && (maxY > rect.top() && minY < rect.bottom())) {
                return true;
            }
        }

        // Стандартное пересечение интервалов (внутрь прямоугольника заходить нельзя)
        // "проход через нулевую ширину или высоту не считается пересечением"
        if (isZeroWidth && isZeroHeight) {
            return false; // Точечный прямоугольник не блокирует
        }

        // Внутренняя область (строгое пересечение или нахождение внутри)
        bool overlapX = (minX < rect.right() && maxX > rect.left());
        bool overlapY = (minY < rect.bottom() && maxY > rect.top());

        if (isZeroWidth) {
            // Вертикальный отрезок-стена: пересекает, если наш отрезок горизонтально пересекает его X
            return (minX <= rect.left() && maxX >= rect.left() && overlapY);
        }
        if (isZeroHeight) {
            // Горизонтальный отрезок-стена
            return (minY <= rect.top() && maxY >= rect.top() && overlapX);
        }

        return (overlapX && overlapY);
    }

    // Проверка всего пути на валидность
    static bool isValidPath(const QVector<QPoint>& path, const QRect& srcOver, const QRect& dstOver) {
        for (int i = 0; i < path.size() - 1; ++i) {
            if (intersectsRect(path[i], path[i+1], srcOver) ||
                intersectsRect(path[i], path[i+1], dstOver)) {
                return false;
            }
        }
        return true;
    }

    // Вычисление длины ломаной линии
    static int getPathLength(const QVector<QPoint>& path) {
        int length = 0;
        for (int i = 0; i < path.size() - 1; ++i) {
            length += std::abs(path[i].x() - path[i+1].x()) + std::abs(path[i].y() - path[i+1].y());
        }
        return length;
    }

    // Основная функция трассировки
    static QVector<QPoint> findRoute(QPoint srcPoint, QPoint dstPoint,
                                     QRect srcOver, QRect dstOver, QPoint grid) {
        QVector<QPoint> bestPath;
        int minLength = INT_MAX;

        // Направления для первого шага (0 - горизонтально, 1 - вертикально)
        for (int initialDir : {0, 1}) {

            // Генерируем возможные ключевые координаты (каналы трассировки)
            QVector<int> xChannels = { srcPoint.x(), dstPoint.x() };
            QVector<int> yChannels = { srcPoint.y(), dstPoint.y() };

            // Добавляем отступы вокруг препятствий (на безопасное расстояние сетки)
            auto addObstacleChannels = [&](const QRect& r) {
                if (!r.isNull() && r.isValid()) {
                    int stepX = grid.x() > 0 ? grid.x() : 10;
                    int stepY = grid.y() > 0 ? grid.y() : 10;
                    xChannels << snap(r.left() - stepX, stepX) << snap(r.right() + stepX, stepX);
                    yChannels << snap(r.top() - stepY, stepY) << snap(r.bottom() + stepY, stepY);
                }
            };
            addObstacleChannels(srcOver);
            addObstacleChannels(dstOver);

            // Перебор возможных промежуточных изломов (до 5 сегментов = до 4 промежуточных точек)
            for (int cx1 : xChannels) {
                for (int cy1 : yChannels) {
                    for (int cx2 : xChannels) {
                        for (int cy2 : yChannels) {

                            QVector<QPoint> path;
                            path.append(srcPoint);

                            // --- СТРОИМ ТОЧКУ 1 (Первый излом) ---
                            // Выравниваем ОДНУ координату по сетке в зависимости от направления
                            QPoint p1;
                            if (initialDir == 0) { // Движение по X
                                p1.setX(snap(cx1, grid.x())); // Первая координата выровнена
                                p1.setY(srcPoint.y());        // Вторая сохраняет исходное положение
                            } else { // Движение по Y
                                p1.setX(srcPoint.x());
                                p1.setY(snap(cy1, grid.y()));
                            }
                            if (p1 != path.last()) path.append(p1);

                            // --- СТРОИМ ТОЧКУ 2 (Второй излом) ---
                            // На втором изломе выравнивается ВТОРАЯ координата по сетке.
                            // Теперь вся траектория жестко сидит на узлах сетки.
                            QPoint p2;
                            if (initialDir == 0) {
                                p2.setX(p1.x()); // Уже выровнен по X
                                p2.setY(snap(cy2, grid.y())); // Теперь выровнен и по Y
                            } else {
                                p2.setX(snap(cx2, grid.x()));
                                p2.setY(p1.y());
                            }
                            if (p2 != path.last()) path.append(p2);

                            // --- СТРОИМ ПРОМЕЖУТОЧНЫЕ ТОЧКИ ДЛЯ ВЫХОДА НА ФИНИШ ---
                            // Точка 3 для ортогонального перехода к финишу
                            QPoint p3;
                            if (initialDir == 0) {
                                p3.setX(snap(dstPoint.x(), grid.x()));
                                p3.setY(p2.y());
                            } else {
                                p3.setX(p2.x());
                                p3.setY(snap(dstPoint.y(), grid.y()));
                            }
                            if (p3 != path.last()) path.append(p3);

                            // Добавляем финальную точку
                            if (dstPoint != path.last()) path.append(dstPoint);

                            // Проверяем ограничения: сегментов должно быть от 1 до 5 (точек от 2 до 6)
                            int segments = path.size() - 1;
                            if (segments >= 1 && segments <= 5) {
                                if (isValidPath(path, srcOver, dstOver)) {
                                    int len = getPathLength(path);
                                    if (len < minLength) {
                                        minLength = len;
                                        bestPath = path;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return bestPath;
    }
};
#endif

