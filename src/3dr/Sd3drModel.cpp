/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  3d object which represents face set model (Model which consists of face set)
  Each face is flate surface bounded polyline
  With comparison Sd3dFaceSet this object contains simplified faces (no normal, single face color)
*/
#include "Sd3drModel.h"
#include "SvLib/SvJsonIO.h"
#include "Sd3drPointLink.h"

#include <QLineF>
#include <QPolygonF>
#include <math.h>





Sd3ColorList Sd3drModel::colorList() const
  {
  Sd3ColorList list;
  for( auto const &inst : mInstanceList )
    inst.bodyColorGet( list );
  return list;
  }



void Sd3drModel::colorListSet(const Sd3ColorList &lst)
  {
  int index = 0;
  for( int i = 0; i < mInstanceList.count(); i++ )
    mInstanceList[i].bodyColorSet( lst, index );
  }



void Sd3drModel::clear()
  {
  mVertexList.clear();
  mInstanceList.clear();
  }




Sd2dRegion Sd3drModel::flatRectangle(float lenght, float width)
  {
  Sd2dRegion r;
  width /= 2.0;
  lenght /= 2.0;
  r.append( QVector2D(-lenght,  width) );
  r.append( QVector2D( lenght,  width) );
  r.append( QVector2D( lenght, -width) );
  r.append( QVector2D(-lenght, -width) );
  return r;
  }




Sd2dRegion Sd3drModel::flatRectangleBevel(float lenght, float width, float bevelSize, float count)
  {
  if( count <= 0 )
    return flatRectangle( lenght, width );
  Sd2dRegion r;
  width /= 2.0;
  lenght /= 2.0;
  r.append( QVector2D(-lenght + bevelSize,  width) );
  if( count < 2 )
    r.append( QVector2D( lenght,  width) );
  else {
    r.append( QVector2D( lenght - bevelSize,  width) );
    r.append( QVector2D( lenght,  width - bevelSize ) );
    }
  if( count < 3 )
    r.append( QVector2D( lenght, -width) );
  else {
    r.append( QVector2D( lenght, -width + bevelSize ) );
    r.append( QVector2D( lenght - bevelSize, -width) );
    }
  if( count < 4 )
    r.append( QVector2D(-lenght, -width) );
  else {
    r.append( QVector2D(-lenght + bevelSize, -width) );
    r.append( QVector2D(-lenght, -width + bevelSize) );
    }
  r.append( QVector2D(-lenght,  width - bevelSize) );
  return r;
  }





Sd2dRegion Sd3drModel::flatRectangleRound(float lenght, float width, float radius, float stepDegree, float count)
  {
  Sd2dRegion region;
  width /= 2.0;
  lenght /= 2.0;
  float curX = lenght - radius;
  float curY = width - radius;
  //Top right corner
  for( float angleDegree = 0; angleDegree <= 90.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector2D v( sin(angle) * radius + curX, cos(angle) * radius + curY );
    //Append corner to region
    region.append( v );
    }

  //Right edge
  if( count < 2 )
    region.append( QVector2D( lenght, -width) );
  else {
    curY = -curY;
    region.append( QVector2D( curX + radius, curY )  );

    //Bottom right corner
    for( float angleDegree = 90.0 + stepDegree; angleDegree <= 180.0; angleDegree += stepDegree ) {
      //Convert degree to radians
      float angle = angleDegree * M_PI / 180.0;
      //Build next corner
      QVector2D v( sin(angle) * radius + curX, cos(angle) * radius + curY );
      //Append corner to region
      region.append( v );
      }
    }

  //Bottom edge
  if( count < 3 )
    region.append( QVector2D(-lenght, -width) );
  else {
    curX = -curX;
    region.append( QVector2D( curX, curY - radius )  );

    //Bottom left corner
    for( float angleDegree = 180.0 + stepDegree; angleDegree <= 270.0; angleDegree += stepDegree ) {
      //Convert degree to radians
      float angle = angleDegree * M_PI / 180.0;
      //Build next corner
      QVector2D v( sin(angle) * radius + curX, cos(angle) * radius + curY );
      //Append corner to region
      region.append( v );
      }
    }

  //Left edge
  if( count < 4 )
    region.append( QVector2D(-lenght,  width) );
  else {
    curY = -curY;
    region.append( QVector2D( curX - radius, curY )  );

    //Top left corner
    for( float angleDegree = 270.0 + stepDegree; angleDegree <= 360.0; angleDegree += stepDegree ) {
      //Convert degree to radians
      float angle = angleDegree * M_PI / 180.0;
      //Build next corner
      QVector2D v( sin(angle) * radius + curX, cos(angle) * radius + curY );
      //Append corner to region
      region.append( v );
      }
    }

  //Top edge is automatic as closed edge

  return region;
  }




Sd2dRegion Sd3drModel::flatEllipse(float radiusx, float radiusy, float stepDegree)
  {
  Sd2dRegion region;
  //Build circle with step degree
  for( float angleDegree = 0; angleDegree < 360.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector2D v( sin(angle) * radiusx, cos(angle) * radiusy );
    //Append corner to region
    region.append( v );
    }
  return region;
  }





Sd2dRegion Sd3drModel::flatCircle(float radius)
  {
  return flatEllipse( radius, radius, 360.0 / 30.0 );
  }



Sd2dRegion Sd3drModel::flatPlygedronInner(float radius, float sideCount)
  {
  if( sideCount < 3.0 ) return flatCircle( radius );
  float R = radius / cos( M_PI / sideCount );
  return flatEllipse( R, R, 360.0 / sideCount );
  }



Sd2dRegion Sd3drModel::flatPlygedronOuter(float radius, float sideCount)
  {
  return flatEllipse( radius, radius, 360.0 / sideCount );
  }



Sd2dRegion Sd3drModel::flatTrapezoid(float lenghtTop, float lenghtBot, float width)
  {
  Sd2dRegion r;
  width /= 2.0;
  lenghtTop /= 2.0;
  lenghtBot /= 2.0;
  r.append( QVector2D(-lenghtTop,  width) );
  r.append( QVector2D( lenghtTop,  width) );
  r.append( QVector2D( lenghtBot, -width) );
  r.append( QVector2D(-lenghtBot, -width) );
  return r;
  }



Sd2dRegion Sd3drModel::flatTrapezoidRound(float lenghtTop, float lenghtBot, float width, float radius)
  {
  Sd2dRegion region;
  width /= 2.0;
  lenghtTop /= 2.0;
  lenghtBot /= 2.0;
  float topAngle,botAngle;
  float stepDegree = 360.0 / 30.0;
  if( lenghtTop < lenghtBot ) {
    float acuteRad = atan2( width, (lenghtBot - lenghtTop) );
    topAngle = acuteRad * 180.0 / M_PI;
    botAngle = 180 - topAngle;
    }
  else {
    float acuteRad = atan2( width, (lenghtTop - lenghtBot) );
    botAngle = acuteRad * 180.0 / M_PI;
    topAngle = 180 - botAngle;
    }
  float curX = lenghtTop - radius;
  float curY = width - radius;
  float angleDegree;
  //Top right corner
  for( angleDegree = 0; angleDegree <= topAngle; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector2D v( sin(angle) * radius + curX, cos(angle) * radius + curY );
    //Append corner to region
    region.append( v );
    }

  //Right edge
  curX = lenghtBot - radius;
  curY = -curY;
  //region.append( QVector2D( curX + radius, curY )  );

  //Bottom right corner
  for( ; angleDegree <= 180.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector2D v( sin(angle) * radius + curX, cos(angle) * radius + curY );
    //Append corner to region
    region.append( v );
    }

  //Bottom edge
  curX = -curX;
  region.append( QVector2D( curX, curY - radius )  );

  //Bottom left corner
  for( ; angleDegree <= (180.0 + botAngle); angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector2D v( sin(angle) * radius + curX, cos(angle) * radius + curY );
    //Append corner to region
    region.append( v );
    }

  //Left edge
  curX = -lenghtTop + radius;
  curY = -curY;
  //region.append( QVector2D( curX - radius, curY )  );

  //Top left corner
  for( ; angleDegree <= 360.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector2D v( sin(angle) * radius + curX, cos(angle) * radius + curY );
    //Append corner to region
    region.append( v );
    }

  //Top edge is automatic as closed edge

  return region;
  }





static const double EPSILON = 1e-10;

// Сравнение чисел с плавающей точкой
static bool isZero(double val)
  {
  return fabs(val) < EPSILON;
  }



static bool isEqual(double a, double b)
  {
  return isZero(a - b);
  }



static bool isEqual(const QVector2D& p1, const QVector2D& p2)
  {
  return isZero(p1.x() - p2.x()) && isZero(p1.y() - p2.y());
  }


// Нормализация вектора
static QVector2D normalize(const QVector2D& v)
  {
  double len = sqrt(v.x() * v.x() + v.y() * v.y());
  if (len < EPSILON) return QVector2D(0, 0);
  return QVector2D(v.x() / len, v.y() / len);
  }


// Поворот вектора на 90 градусов против часовой стрелки
static QVector2D rotate90(const QVector2D& v)
  {
  return QVector2D(-v.y(), v.x());
  }

// Получение нормали к ребру
static QVector2D getEdgeNormal(const QVector2D& p1, const QVector2D& p2)
  {
  QVector2D edge = p2 - p1;
  if( isZero(edge.x()) && isZero(edge.y()) ) {
    return QVector2D(0, 0);  // Ребро нулевой длины
    }
  return rotate90(normalize(edge));
  }

// Определение ориентации полигона
static bool isCounterClockwise(const Sd2dRegion& polygon)
  {
  double area = 0;
  int n = polygon.size();
  for( int i = 0; i < n; ++i ) {
    int j = (i + 1) % n;
    area += polygon[i].x() * polygon[j].y() - polygon[j].x() * polygon[i].y();
    }
  return area > 0;
  }

// Проверка, лежит ли точка между двумя другими на прямой
static bool isPointBetween(const QVector2D& p, const QVector2D& a, const QVector2D& b)
  {
  if( !isZero((b.x() - a.x()) * (p.y() - a.y()) - (b.y() - a.y()) * (p.x() - a.x())) ) {
    return false;  // Точка не на прямой
    }

  double dot = (p.x() - a.x()) * (b.x() - a.x()) + (p.y() - a.y()) * (b.y() - a.y());
  if( dot < -EPSILON ) return false;

  double squaredLength = (b.x() - a.x()) * (b.x() - a.x()) + (b.y() - a.y()) * (b.y() - a.y());
  if (isZero(squaredLength)) return isEqual(p, a);

  return dot <= squaredLength + EPSILON;
  }

// Вычисление угла между векторами в градусах
static double angleBetween(const QVector2D& v1, const QVector2D& v2)
  {
  double dot = QVector2D::dotProduct(v1, v2);
  double len1 = sqrt(v1.x() * v1.x() + v1.y() * v1.y());
  double len2 = sqrt(v2.x() * v2.x() + v2.y() * v2.y());

  if( isZero(len1) || isZero(len2) ) return 0;

  double cosAngle = dot / (len1 * len2);
  cosAngle = qMax(-1.0, qMin(1.0, cosAngle));

  return acos(cosAngle) * 180.0 / M_PI;
  }



struct EdgeInfo
  {
    QVector2D p1, p2;   // Исходные точки
    QVector2D op1, op2; // Смещенные точки
    bool      isValid;  // Ребро валидно (ненулевой длины)
    QVector2D normal;   // Нормаль к ребру

    EdgeInfo() : isValid(false) {}
  };

Sd2dRegion Sd3drModel::flatEquidistant(const Sd2dRegion &r, float distance)
  {
  if( r.size() < 3 ) {
    return Sd2dRegion{};  // Пустой регион для неправильного полигона
    }

  int n = r.size();

  //Определяем направление смещения
  bool isCCW = isCounterClockwise(r);
  double sign = isCCW ? 1.0 : -1.0;

  //Собираем информацию о ребрах, пропуская ребра нулевой длины
  QList<EdgeInfo> edges;
  QList<int> validIndices;  // Индексы вершин, где начинаются валидные ребра

  for( int i = 0; i < n; ++i ) {
    int next = (i + 1) % n;

    EdgeInfo edge;
    edge.p1 = r[i];
    edge.p2 = r[next];

    //Проверка на ребро нулевой длины
    if( isEqual(edge.p1, edge.p2) ) {
      edge.isValid = false;
      }
    else {
      edge.isValid = true;
      edge.normal = getEdgeNormal(edge.p1, edge.p2);
      edge.op1 = edge.p1 + edge.normal * distance * sign;
      edge.op2 = edge.p2 + edge.normal * distance * sign;
      validIndices.append(i);
      }

    edges.append(edge);
    }

  // Если нет ни одного валидного ребра
  if( validIndices.isEmpty() ) {
    return Sd2dRegion{};
    }

  // Если только одно валидное ребро
  if( validIndices.size() == 1 ) {
    Sd2dRegion result;
    int idx = validIndices[0];
    result.append(edges[idx].op1);
    result.append(edges[idx].op2);
    return result;
    }

  Sd2dRegion result;

  // Обрабатываем каждую валидную вершину (где сходятся два валидных ребра)
  for (int i = 0; i < validIndices.size(); ++i) {
    int currIdx = validIndices[i];
    int prevIdx = validIndices[(i - 1 + validIndices.size()) % validIndices.size()];

    // Проверяем, что ребра действительно смежные в исходном полигоне
    // (между ними могут быть ребра нулевой длины)
    bool areAdjacent = false;
    int testIdx = prevIdx;
    while( testIdx != currIdx ) {
      testIdx = (testIdx + 1) % n;
      if (testIdx == currIdx) {
        areAdjacent = true;
        break;
        }
      if( edges[testIdx].isValid ) {
        break;  // Нашли другое валидное ребро - значит не смежные
        }
      }

    if( !areAdjacent ) {
      continue;  // Пропускаем несмежные ребра
      }

    const EdgeInfo& edge1 = edges[prevIdx];
    const EdgeInfo& edge2 = edges[currIdx];

    // Проверка на угол 0 градусов между ребрами
    QVector2D v1 = normalize(edge1.p1 - edge1.p2);
    QVector2D v2 = normalize(edge2.p2 - edge2.p1);
    double angle = angleBetween(v1, v2);

    if (isZero(angle)) {
      qDebug() << "Error: Zero angle between edges";
      return Sd2dRegion{};  // Возвращаем пустой регион
      }

    QLineF line1(edge1.op1.toPointF(), edge1.op2.toPointF());
    QLineF line2(edge2.op1.toPointF(), edge2.op2.toPointF());

    QPointF intersection;
    QLineF::IntersectType type = line1.intersects(line2, &intersection);

    // Особый случай: угол 180 градусов (ребра образуют прямую линию)
    if( isEqual(angle, 180.0) ) {
      // Используем смещенную точку первого ребра
      if( !result.isEmpty() && !isEqual(result.last(), edge1.op2) ) {
        result.append(edge1.op2);
        }
      continue;
      }

    if( type == QLineF::NoIntersection ) {
      // Параллельные линии - проверяем особый случай offsetP1 == offsetP2
      if( isEqual(edge1.op2, edge2.op1) ) {
        if( result.isEmpty() || !isEqual(result.last(), edge1.op2) ) {
          result.append(edge1.op2);
          }
        continue;
        }

      qDebug() << "Error: Parallel edges with no intersection";
      return Sd2dRegion();  // Возвращаем пустой регион
      }

    // Проверяем плохой случай: одна из смещенных точек лежит между соседним ребром и смещенным
    // QPointF testPoint = (type == QLineF::BoundedIntersection) ? intersection :
    //                                                             (i == 0 ? edge1.op2 : edge2.op1);

    // Проверяем для первого ребра
    if (isPointBetween(edge1.op2, edge2.op1, edge2.op2) &&
        !isEqual(edge1.op2, edge2.op1) && !isEqual(edge1.op2, edge2.op2)) {
      qDebug() << "Error: Offset point lies between adjacent edge and its offset";
      return Sd2dRegion();
      }

    // Проверяем для второго ребра
    if (isPointBetween(edge2.op1, edge1.op1, edge1.op2) &&
        !isEqual(edge2.op1, edge1.op1) && !isEqual(edge2.op1, edge1.op2)) {
      qDebug() << "Error: Offset point lies between adjacent edge and its offset";
      return Sd2dRegion();
      }

    // Добавляем точку пересечения
    if (result.isEmpty() || !isEqual(result.last(), QVector2D(intersection) )) {
      result.append( QVector2D(intersection) );
      }
    }

  return result;
  }




Sd2dRegion Sd3drModel::flatOffset(const Sd2dRegion &r, float offx, float offy)
  {
  Sd2dRegion res;
  res.reserve( r.size() );
  QVector2D off(offx,offy);
  for( const auto &v : r )
    res.append( v + off );
  return res;
  }




Sd2dRegion Sd3drModel::flatCircularSegment(float radius, float angleStart, float angleStop)
  {
  const int stepCount = 16;
  Sd2dRegion list;
  float angleStep = (angleStop - angleStart) / stepCount;
  //Convert degree to radians
  float angle = angleStart * M_PI / 180.0;
  QVector2D p( sin(angle) * radius, cos(angle) * radius );
  list.append( p );
  for( int i = 0; i < stepCount; i++ ) {
    angleStart += angleStep;
    angle = angleStart * M_PI / 180.0;
    QVector2D p( sin(angle) * radius, cos(angle) * radius );
    list.append( p );
    }
  return list;
  }



Sd2dRegion Sd3drModel::flatFromFace(const Sd3drFace &face, const QMatrix4x4 &map)
  {
  Sd2dRegion region2D;

  // Вычисляем обратную матрицу
  QMatrix4x4 invMatrix = map.inverted();

  // Применяем обратное преобразование к каждой точке
  for( int pointIndex : face ) {
    // Преобразуем точку обратно в локальные координаты
    QVector3D localPoint = invMatrix.map( vertex(pointIndex) );

    // Берем только X и Y координаты (исходный 2D регион лежал в плоскости XY)
    region2D.append(QVector2D(localPoint.x(), localPoint.y()));
    }

  return region2D;
  }





Sd3drFace Sd3drModel::flatToFace(const Sd2dRegion &r, float offz)
  {
  Sd3drFace face;
  for( auto v2 : r )
    face.append( vertexAppend( QVector3D(v2,offz) )  );
  return face;
  }








Sd3drRegion Sd3drModel::vertexList(const QList<int> &indexList) const
  {
  Sd3drRegion region;
  for( auto index : indexList )
    region.append( vertex(index) );
  return region;
  }




QMatrix4x4 Sd3drModel::matrixShift(const Sd3drFace &face, float shift)
  {
  QMatrix4x4 map;
  if( face.count() < 3 )
    return map;
  //Normal vector
  QVector3D normal = QVector3D::normal( vertex( face.at(0) ), vertex( face.at(1) ), vertex( face.at(2) ) );
  normal *= -shift;
  map.translate( normal );
  return map;
  }



Sd3drFace Sd3drModel::faceFromFlat(const Sd2dRegion &r, const QMatrix4x4 &map, float offset )
  {
  // Получаем локальную ось Z из матрицы (нормаль в мировых координатах)
  QVector3D localZ(map(0, 2), map(1, 2), map(2, 2));
  // Смещаем точку вдоль этой оси
  localZ *= offset;

  Sd3drFace face;
  for( auto v2 : r )
    face.append( vertexAppend( localZ + map.map( QVector3D(v2) ))  );
  return face;
  }




Sd3drFace Sd3drModel::faceFromRegion( const Sd3drRegion &r )
  {
  Sd3drFace regionRef;
  for( auto v : r ) {
    int vertexIndex = vertexAppend( v );
    regionRef.append(vertexIndex);
    }
  return regionRef;
  }




//!
//! \brief faceFlat         Builds flat face from list of pairs floats
//! \param firstVertexIndex First vertex index from which builds face
//! \param pairList         Pair list as offset from current point begining from firstVertexIndex
//! \param orientation      0 - xy, 1 - yz, 2 - xz
//! \return
//!
Sd3drFace Sd3drModel::faceFlat( int firstVertexIndex, const QList<float> &pairList, int orientation)
  {
  Sd3drFace face;
  float xy( orientation == 0 ? 1.0 : 0 );
  float yz( orientation == 1 ? 1.0 : 0 );
  float xz( orientation == 2 ? 1.0 : 0 );
  int count = pairList.count() / 2;
  if( count < 2 )
    return face;
  count <<= 1;
  QVector3D v( vertex(firstVertexIndex) );
  face.append(firstVertexIndex);
  for( int i = 0; i < count; i += 2 ) {
    QVector3D dv( pairList.at(i) * xy + pairList.at(i) * xz, pairList.at(i+1) * xy + pairList.at(i) * yz, pairList.at(i+1) * yz + pairList.at(i+1) * xz );
    v += dv;
    face.append( vertexAppend(v) );
    }
  return face;
  }



//!
//! \brief faceFlatMatrix Builds flat face from list of pairs float. First pair is source of path and next pairs are offsets from previous pair
//! \param pairList       List of pairs float. First pair is source of path and next pairs are offsets from previous pair
//! \param map            Transfer map
//! \return               Face
//!
Sd3drFace Sd3drModel::faceFlatMatrix(const QList<float> &pairList, const QMatrix4x4 &map)
  {
  Sd3drFace face;
  int count = pairList.count() / 2;
  if( count < 3 )
    return face;
  count <<= 1;
  QVector3D v( pairList.at(0), pairList.at(1), 0 );
  face.append( vertexAppend( map.map(v) ) );
  for( int i = 2; i < count; i += 2 ) {
    QVector3D dv( pairList.at(i), pairList.at(i+1), 0 );
    v += dv;
    face.append( vertexAppend( map.map(v) ) );
    }
  return face;
  }



//!
//! \brief faceCircleThis function builds circle region on base radius with center at 0
//! \param radius       Radius of builded circle
//! \param stepDegree   Step with which need to create multicorner circle region
//! \param map          Finish circle transformation map
//! \return             Circle
//!
Sd3drFace Sd3drModel::faceCircle(float radius, float stepDegree, const QMatrix4x4 &map)
  {
  return faceEllipse( radius, radius, stepDegree, map );
  }



//!
//! \brief faceCircleSide Builds circle region interpolated polygon on base radius with center at 0 and sideCount sides
//! \param radius         Radius of builded circle
//! \param sideCount      Side count of polygon
//! \param map            Finish circle transformation map
//! \return               Circle
//!
Sd3drFace Sd3drModel::faceCircleSide(float radius, int sideCount, const QMatrix4x4 &map)
  {
  float stepDegree = 360.0 / sideCount;
  return faceCircle( radius, stepDegree, map );
  }




Sd3drFace Sd3drModel::faceEllipse(float radiusx, float radiusy, float stepDegree, const QMatrix4x4 &map)
  {
  return faceFromFlat( flatEllipse( radiusx, radiusy, stepDegree ), map );
  }




Sd3drFace Sd3drModel::faceEllipseSide(float radiusx, float radiusy, int sideCount, const QMatrix4x4 &map)
  {
  float stepDegree = 360.0 / sideCount;
  return faceEllipse( radiusx, radiusy, stepDegree, map );
  }



//!
//! \brief faceRectangle Builds rectangle region with center at 0 and four edges
//! \param lenght        Lenght of rectangle (X)
//! \param width         Width of rectangle (Y)
//! \param map           Finish rectangle transformation map
//! \return              Rectangle
//!
Sd3drFace Sd3drModel::faceRectangle(float lenght, float width, const QMatrix4x4 &map)
  {
  width /= 2.0;
  lenght /= 2.0;
  QVector3D p0( -lenght, -width, 0 );
  QVector3D p1(  lenght, -width, 0 );
  QVector3D p2(  lenght,  width, 0 );
  QVector3D p3( -lenght,  width, 0 );

  return Sd3drFace( { vertexAppend( map.map(p0) ), vertexAppend( map.map(p1)), vertexAppend( map.map(p2) ), vertexAppend( map.map(p3) ) } );
  }




Sd3drFace Sd3drModel::faceRectangleRight(float lenght, float width, const QMatrix4x4 &map)
  {
  return faceFromFlat( flatRectangle( lenght, width ), map );
  }



Sd3drFace Sd3drModel::faceRectangleRound(float lenght, float width, float radius, float stepDegree, const QMatrix4x4 &map)
  {
  return faceFromFlat( flatRectangleRound( lenght, width, radius, stepDegree, 4 ), map );
  }






//!
//! \brief faceFromUmPolygon Builds face from region defined in um
//! \param poly              Source region
//! \param map               Map to convert result
//! \return                  Face
//!
Sd3drFace Sd3drModel::faceFromUmPolygon(const QPolygonF &poly, const QMatrix4x4 &map)
  {
  Sd3drFace region;
  //Convert microns into mm
  for( const QPointF &p : poly ) {
    QVector3D v( p / 1000.0 );
    //Append corner to region
    region.append( vertexAppend( map.map(v) )  );
    }

  return region;
  }




//!
//! \brief faceRectangleSide Builds rectangle region with center at 0 and sideCount sides reorganized to rectangle
//! \param width             Lenght of rectangle (X)
//! \param lenght            Width of rectangle (Y)
//! \param sideCount         Side count of polygon
//! \param map               Finish rectangle transformation map
//! \return                  Rectangle
//!
Sd3drFace Sd3drModel::faceRectangleSide(float lenght, float width, int sideCount, const QMatrix4x4 &map)
  {
  float edgeOnSide = sideCount / 4;
  float stepw = lenght / edgeOnSide;
  float steph = width / edgeOnSide;
  QVector3D v( 0, lenght/2.0, 0);
  Sd3drFace face;
  //face.append( vertexAppend( map.map( v ) )   );
  for( int i = 0; i < edgeOnSide / 2; i++ ) {
    v += QVector3D( stepw, 0, 0 );
    face.append( vertexAppend( map.map( v ) )   );
    }
  for( int i = 0; i < edgeOnSide; i++ ) {
    v += QVector3D( 0, -steph, 0 );
    face.append( vertexAppend( map.map( v ) )   );
    }
  for( int i = 0; i < edgeOnSide; i++ ) {
    v += QVector3D( -stepw, 0, 0 );
    face.append( vertexAppend( map.map( v ) )   );
    }
  for( int i = 0; i < edgeOnSide; i++ ) {
    v += QVector3D( 0, steph, 0 );
    face.append( vertexAppend( map.map( v ) )   );
    }
  for( int i = 0; i < edgeOnSide / 2; i++ ) {
    v += QVector3D( stepw, 0, 0 );
    face.append( vertexAppend( map.map( v ) )   );
    }
  return face;
  }




Sd3drFace Sd3drModel::faceDuplicate(const Sd3drFace &face, const QMatrix4x4 &map)
  {
  Sd3drFace dest;
  dest.reserve(face.count());
  for( auto index : face )
    dest.append( vertexAppend( map.map(vertex(index)) )  );
  return dest;
  }




Sd3drFace Sd3drModel::faceDuplicateScale(const Sd3drFace &face, float scalex, float scaley, float shift)
  {
  QMatrix4x4 map;
  map.translate( 0, 0, shift );
  map.scale( scalex, scaley, 1.0 );
  return faceDuplicate( face, map );
  }




Sd3drFace Sd3drModel::faceDuplicateGrow(const Sd3drFace &face, float deltax, float deltay, const QMatrix4x4 &map)
  {
  //Calculate lenght (x) and width (y) of face
  //Calculate dx and dy scale
  if( face.count() < 3 )
    return face;
  //Calculate scale factor
  float sizex,sizey;
  faceSizeXY( face, sizex, sizey );
  QMatrix4x4 mp(map);
  mp.scale( (sizex + deltax) / sizex, (sizey + deltay) / sizey, 1.0 );
  return faceDuplicate( face, mp );
  }




Sd3drFace Sd3drModel::faceDuplicateOffset(const Sd3drFace &face, QVector3D offset)
  {
  Sd3drFace out;
  out.reserve( face.size() );
  for( int i : face )
    out.append( vertexAppend( vertex(i) + offset ) );
  return out;
  }





Sd3drFace Sd3drModel::faceTrapezoidZ(const Sd3drFace &face, float height, float lessX, float lessY, float topShiftX, float topShiftY)
  {
  float x(0), y(0);
  for( auto index : face ) {
    QVector3D v = vertex( index );
    x = qMax( x, qAbs(v.x()) );
    y = qMax( y, qAbs(v.y()) );
    }
  float topX = x - lessX;
  float topY = y - lessY;
  if( x == 0 || y == 0 )
    return Sd3drFace{};
  QMatrix4x4 map;
  map.scale( topX / x, topY / y, 1.0 );
  map.translate( topShiftX, topShiftY, height );
  return faceDuplicate( face, map );
  }




Sd3drFace Sd3drModel::faceCurveXZ(const Sd3drFace &face, float radius, float angleSrc, float angleDst)
  {
  //Find center of face
  QVector3D center;
  for( auto const &v : face )
    center += vertex(v);
  float count = face.count();
  center = center / count;

  QVector3D rotationCenter( radius, 0.0, 0.0 );
  QMatrix4x4 rotationMap;
  rotationMap.rotate( angleSrc, 0.0, 1.0, 0.0 );
  center += rotationMap.map( rotationCenter );

  QMatrix4x4 centerMap;
  //Reverse order of transformations
  centerMap.translate( center );
//  centerMap.translate( radius, 0 );
  centerMap.rotate( angleDst, 0.0, 1.0, 0.0 );
//  centerMap.translate( -radius, 0 );
//  centerMap.rotate( -angleSrc, 0.0, 1.0, 0.0 );
  centerMap.translate( -center.x(), -center.y(), -center.z() );


  Sd3drFace dest;
  dest.reserve( face.count() );
  for( auto const &v : face )
    dest.append( vertexAppend( centerMap.map(vertex(v)) )  );

  return dest;
  }



//!
//! \brief faceEqudistante Calculate equidistant face of give face with distance. At last make transformation with map
//! \param face            Source face
//! \param distance        Distance of new face
//! \param map             Finish translation map
//! \return                Equidistant face
//!
Sd3drFace Sd3drModel::faceEqudistanteXY(const Sd3drFace &face, float distance, const QMatrix4x4 &map)
  {
  if( face.count() < 3 )
    return face;

  //Fill contour with segments
  QList<QLineF> eq;
  eq.reserve( face.count() );
  QLineF line;
  //First segment
  line.setP1( point(face.last()) );
  line.setP2( point(face.first()) );
  eq.append( line );
  for( int i = 1; i < face.count(); i++ ) {
    line.setP1( point(face.at(i-1)) );
    line.setP2( point(face.at(i)) );
    eq.append( line );
    }

  //For each segment calculate paralled line
  for( int i = 0; i < face.count(); i++ ) {
    line = eq.at(i);
    float dx = line.dx();
    float dy = line.dy();
    float len = line.length();
    if( len != 0 ) {
      float px = distance * dy / len;
      float py = distance * dx / len;
      eq[i].translate( px, -py );
      }
    }

  //For each segment calculate intersection point with next segment and place point into result face
  Sd3drFace faceEq;
  QPointF v;
  for( int i = 1; i < eq.count(); i++ ) {
    if( eq[i].intersects( eq.at(i-1), &v ) == QLineF::NoIntersection )
      v = eq.at(i).p1();
    faceEq.append( vertexAppend( map.map( QVector3D(v))) );
    }
  //Last segment
  if( eq.first().intersects( eq.last(), &v ) == QLineF::NoIntersection )
    v = eq.first().p1();
  faceEq.append( vertexAppend( map.map( QVector3D(v))) );
  return faceEq;
  }




//!
//! \brief faceBevelXY Calculate finish round face of given face with radius and with shift with given direction
//! \param face        Source face
//! \param radius      Distance of new face
//! \return            Shifted round face
//!
Sd3drFace Sd3drModel::faceBevelXY(const Sd3drFace &face, float radius )
  {
  //Calculate dx and dy scale
  if( face.count() < 3 )
    return face;
  //Calculate scale factor
  float sizex,sizey;
  faceSizeXY( face, sizex, sizey );
  return faceDuplicateScale( face, (sizex + radius) / sizex, (sizey + radius) / sizey, fabs(radius) );
  }





Sd3drFace Sd3drModel::facePart(const Sd3drFace &face, const QList<float> &indexes)
  {
  Sd3drFace dest;
  for( auto v : indexes ) {
    int i = static_cast<int>(v);
    if( i >= 0 && i < face.count() )
      dest.append( face.at(i) );
    }
  return dest;
  }



Sd3drFaceList Sd3drModel::faceListDuplicate(const Sd3drFaceList &faceList, const QMatrix4x4 &map)
  {
  Sd3drFaceList dest;
  for( auto const &face : faceList )
    dest.append( faceDuplicate( face, map ) );
  return dest;
  }



struct SdTriangle
  {
    QPointF mLeftA;
    QPointF mNextB;
    QPointF mPrevC;

    float dyba;
    float dxba;
    float dycb;
    float dxcb;
    float dyac;
    float dxac;
    void prepare()
      {
      dyba = mNextB.y() - mLeftA.y();
      dxba = mNextB.x() - mLeftA.x();

      dycb = mPrevC.y() - mNextB.y();
      dxcb = mPrevC.x() - mNextB.x();

      dyac = mLeftA.y() - mPrevC.y();
      dxac = mLeftA.x() - mPrevC.x();
      }

    bool isPointInside( QPointF p )
      {
      float v0 = (mLeftA.x() - p.x()) * dyba - (mLeftA.y() - p.y()) * dxba;
      float v1 = (mNextB.x() - p.x()) * dycb - (mNextB.y() - p.y()) * dxcb;
      float v2 = (mPrevC.x() - p.x()) * dyac - (mPrevC.y() - p.y()) * dxac;
      return (v0 >= 0 && v1 >= 0 && v2 >= 0) || (v0 < 0 && v1 < 0 && v2 < 0);
      }
  };




Sd3drFaceList Sd3drModel::faceListSimplifyXY( const Sd3drFace &srcFace )
  {
  Sd3drFaceList faceList;
  Sd3drPointLinkList pointPool;
  QList<Sd3drPointLinkPtr> srcList;
  srcList.append( pointPool.addRegion( this, srcFace, false ) );
  for( int faceIndex = 0; faceIndex < srcList.count(); faceIndex++ ) {
    Sd3drPointLinkPtr face = srcList.at(faceIndex);
    while( !face->isTriangle() ) {
      face = face->lessLeft();
      SdTriangle t;
      t.mLeftA = face->mPoint;
      t.mNextB = face->mNext->mPoint;
      t.mPrevC = face->mPrev->mPoint;
      t.prepare();

      //Test all remain points of region
      Sd3drPointLinkPtr inner = nullptr;
      for( Sd3drPointLinkPtr ptr = face->mNext->mNext; ptr != face->mPrev; ptr = ptr->mNext ) {
        if( t.isPointInside( ptr->mPoint ) ) {
          if( inner == nullptr || ptr->isLeft( inner ) )
            inner = ptr;
          }
        }
      if( inner != nullptr ) {
        //Divide source region into two regions
        face->splitRegion( inner, &pointPool );
        srcList.append( inner );
        }
      else {
        //Remove triangle from face
        faceList.append( face->triangle() );
        face = face->remove();
        }
      }
    faceList.append( face->triangle() );
    }
  return faceList;
  }




Sd3drFaceList Sd3drModel::faceListHolesXY(const Sd3drFace &srcFace, const Sd3drFaceList &holeList)
  {
  Sd3drFaceList faceList;
  Sd3drPointLinkList pointPool;
  QList<Sd3drPointLinkPtr> srcList;
  QList<Sd3drPointLinkPtr> srcHole;
  srcList.append( pointPool.addRegion( this, srcFace, true ) );
  for( auto const &hole : holeList )
    srcHole.append( pointPool.addRegion( this, hole, false ) );
  for( int faceIndex = 0; faceIndex < srcList.count(); faceIndex++ ) {
    Sd3drPointLinkPtr face = srcList.at(faceIndex);
    while( !face->isTriangle() ) {
      face = face->lessLeft();
      SdTriangle t;
      t.mLeftA = face->mPoint;
      t.mNextB = face->mNext->mPoint;
      t.mPrevC = face->mPrev->mPoint;
      t.prepare();

      //Test all remain points of region and holes
      Sd3drPointLinkPtr inner = nullptr;
      for( Sd3drPointLinkPtr ptr = face->mNext->mNext; ptr != face->mPrev; ptr = ptr->mNext ) {
        if( t.isPointInside( ptr->mPoint ) ) {
          if( inner == nullptr || ptr->isLeft( inner ) )
            inner = ptr;
          }
        }
      //Test holes
      int usedHole = -1;
      for( int holeIndex = 0; holeIndex < srcHole.count(); holeIndex++ ) {
        //Test one hole
        Sd3drPointLinkPtr hole = srcHole.at(holeIndex);
        Sd3drPointLinkPtr ptr = hole;
        do {
          if( t.isPointInside( ptr->mPoint ) ) {
            if( inner == nullptr || ptr->isLeft( inner ) ) {
              inner = ptr;
              usedHole = holeIndex;
              }
            }
          ptr = ptr->mNext;
          }
        while( ptr != hole );
        }
      if( inner != nullptr ) {
        //Divide source region into two regions or union source with hole
        face->splitRegion( inner, &pointPool );
        if( usedHole >= 0 )
          srcHole.removeAt(usedHole);
        else
          srcList.append( inner );
        }
      else {
        //Remove triangle from face
        faceList.append( face->triangle() );
        face = face->remove();
        }
      }
    faceList.append( face->triangle() );
    }
  return faceList;
  }







//!
//! \brief faceListWalls  Builds walls on base bottom and top regions. Walls builded with color
//! \param face1          First bound of walls - Bottom
//! \param face2          Second bound of walls - Top
//! \param close          If true then append wall with n-1 and 0 index vertex
//! \return               List of walls
//!
Sd3drFaceList Sd3drModel::faceListWall(const Sd3drFace &face1, const Sd3drFace &face2, bool close)
  {
  Sd3drFaceList walls;
  if( face1.count() != face2.count() || face1.count() < 2 )
    return walls;

  for( int i = 0; i < face1.count() - 1; i++ ) {
    Sd3drFace wall( { face1.at(i), face1.at(i+1), face2.at(i+1), face2.at(i) } );
    walls.append( wall );
    }
  if( close ) {
    //Append wall with n-1 and 0 vertex
    Sd3drFace wall( { face1.last(), face1.first(), face2.first(), face2.last() } );
    walls.append( wall );
    }

  return walls;
  }




Sd3drFaceList Sd3drModel::faceListWallList(const Sd3drFaceList &faceList1, const Sd3drFaceList &faceList2, bool close)
  {
  Sd3drFaceList walls;
  if( faceList1.count() != faceList2.count() )
    return walls;

  for( int i = 0; i < faceList1.count(); i++ )
    walls.append( faceListWall( faceList1.at(i), faceList2.at(i), close ) );

  return walls;
  }




Sd3drFaceList Sd3drModel::faceListWallIndexed(const Sd3drFace &face1, const Sd3drFace &face2, const QList<float> &indexes)
  {
  Sd3drFaceList walls;

  for( auto v : indexes ) {
    int i = static_cast<int>(v);
    if( i < face1.count() && i < face2.count() && i >= 0 ) {
      Sd3drFace wall( { face1.at(i), (i+1 < face1.count()) ? face1.at(i+1) : face1.first(), (i+1 < face2.count()) ? face2.at(i+1) : face2.first(), face2.at(i) } );
      walls.append( wall );
      }
    }

  return walls;
  }




Sd3drFaceList Sd3drModel::faceListIndexed(const Sd3drFaceList &faceList, const QList<float> &indexes)
  {
  Sd3drFaceList walls;

  for( auto v : indexes ) {
    int i = static_cast<int>(v);
    if( i < 0 ){
      //Backward counting
      i = faceList.count() + i;
      }
    if( i < faceList.count() && i >= 0 )
      walls.append( faceList.at(i) );
    }

  return walls;
  }


// Проверка, что треугольник (a,b,c) ориентирован против часовой стрелки (положительная площадь)
static bool isCCW(const QVector2D &a, const QVector2D &b, const QVector2D &c)
  {
  return (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x()) > 0;
  }



Sd3drFaceList Sd3drModel::ring(const Sd2dRegion &r1, const Sd3drFace &face1, const Sd2dRegion &r2, const Sd3drFace &face2)
  {
  if( r1.size() < 3 || r2.size() < 3)
    return Sd3drFaceList{};

  // Определяем, какой регион внешний (по площади)
  auto polygonArea = [](const Sd2dRegion &poly) -> float {
    float area = 0;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
      const QVector2D &a = poly[i];
      const QVector2D &b = poly[(i+1)%n];
      area += a.x() * b.y() - a.y() * b.x();
      }
    return area * 0.5f;
    };

  float area1 = polygonArea(r1);
  float area2 = polygonArea(r2);

  const Sd2dRegion *outer, *inner;
  const Sd3drFace  *faceOut, *faceIn;
  if( qAbs(area1) > qAbs(area2) ) {
    outer = &r1;
    inner = &r2;
    faceOut = &face1;
    faceIn = &face2;
    }
  else {
    outer = &r2;
    inner = &r1;
    faceOut = &face2;
    faceIn = &face1;
    }
  // if( area1 > 0 && area2 > 0 ) { // оба по часовой? Пусть внешний — больший по площади
  //   outer = (area1 > area2) ? &r1 : &r2;
  //   inner = (area1 > area2) ? &r2 : &r1;
  //   faceOut = (area1 > area2) ? &face1 : &face2;
  //   faceIn = (area1 > area2) ? &face2 : &face1;
  //   }
  // else if( area1 > 0 ) {
  //   outer = &r1;
  //   inner = &r2;
  //   faceOut = &face1;
  //   faceIn = &face2;
  //   }
  // else {
  //   outer = &r2;
  //   inner = &r1;
  //   faceOut = &face2;
  //   faceIn = &face1;
  //   }

  // Находим ближайшую пару точек между внешним и внутренним контурами
  int innerIdx = 0;
  QVector2D p0( outer->at(0) );
  float minDist = p0.distanceToPoint( inner->at(0) );
  for( int j = 0; j < inner->size(); ++j ) {
    float d = p0.distanceToPoint( inner->at(j) );
    if( d < minDist ) {
      minDist = d;
      innerIdx = j;
      }
    }

  auto map = [faceOut,faceIn] ( int idx ) -> int {
    if( idx < faceOut->size() ) return faceOut->at(idx);
    return faceIn->at(idx - faceOut->size());
    };

  auto point = [outer,inner] (int idx) -> QVector2D {
    if( idx < outer->size() ) return outer->at(idx);
    return inner->at( idx - outer->size() );
    };

  // Строим единый полигон, соединяя внешний и внутренний через разрез
  QList<int> indices;
  indices.reserve( outer->size() + inner->size() + 2 );
  // Добавляем внешний контур
  for( int i = 0; i < outer->size(); ++i )
    indices.append(i);

  // Разрез внутрь
  indices.append( 0 );

  // Внутренний контур (обходим в обратном порядке, чтобы сохранить ориентацию)
  // Внутренний контур (обходим в ОБРАТНОМ порядке)
  // Начинаем с innerIdx и идем назад по индексам
  int i = innerIdx;
  do {
    indices.append( i + outer->size() );
    i--;
    if (i < 0) i = inner->size() - 1;
    }
  while( i != innerIdx );

  // Разрез обратно
  indices.append( innerIdx + outer->size() );

  Sd3drFaceList triangles;

  int step = 0;
  while( indices.size() > 3 ) {
    bool earFound = false;
    for( int i = 0; i < indices.size(); ++i ) {
      int prev = indices[(i - 1 + indices.size()) % indices.size()];
      int curr = indices[i];
      int next = indices[(i + 1) % indices.size()];

      // Проверка, что треугольник (prev, curr, next) — ухо
      QVector2D a = point(prev), b = point(curr), c = point(next);
      if( isCCW(a, b, c) ) continue; // должен быть выпуклый угол

      // Проверка, что внутри треугольника нет других точек
      bool empty = true;
      for( int j = 0; j < indices.size(); ++j ) {
        int p = indices[j];
        if( p == prev || p == curr || p == next ) continue;
        QVector2D pt = point(p);
        // барицентрическая проверка внутри треугольника
        float v0x = c.x() - a.x(), v0y = c.y() - a.y();
        float v1x = b.x() - a.x(), v1y = b.y() - a.y();
        float v2x = pt.x() - a.x(), v2y = pt.y() - a.y();
        float dot00 = v0x*v0x + v0y*v0y;
        float dot01 = v0x*v1x + v0y*v1y;
        float dot02 = v0x*v2x + v0y*v2y;
        float dot11 = v1x*v1x + v1y*v1y;
        float dot12 = v1x*v2x + v1y*v2y;
        float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
        if( u > 0 && v > 0 && u + v < 1 ) {
          empty = false;
          break;
          }
        }
      if( empty ) {
        triangles.append({map(prev), map(curr), map(next)});
        indices.removeAt(i);
        earFound = true;
        break;
        }
      }
    if( !earFound ) break; // защита
    ++step;
    if( step > 10000 ) break; // fallback
    }
  if( indices.size() == 3 ) {
    triangles.append({ map(indices[0]), map(indices[1]), map(indices[2])});
    }
  return triangles;
  }





QList<float> Sd3drModel::afloatArc(float radius, float angleStart, float angleStop, int sideCount )
  {
  QList<float> list;
  if( sideCount < 2 ) sideCount = 2;
  float angleStep = (angleStop - angleStart) / sideCount;
  //Convert degree to radians
  float angle = angleStart * M_PI / 180.0;
  float startx = sin(angle) * radius;
  float starty = cos(angle) * radius;
  for( int i = 0; i < sideCount; i++ ) {
    angleStart += angleStep;
    angle = angleStart * M_PI / 180.0;
    float stopx = sin(angle) * radius;
    float stopy = cos(angle) * radius;
    list.append( stopx - startx );
    list.append( stopy - starty );
    startx = stopx;
    starty = stopy;
    }
  return list;
  }




Sd3drFace Sd3drModel::faceColor(QColor color)
  {
  Sd3drFace face;
  if( color.isValid() ) {
    int cv = color.blue() & 0xff;
    cv |= (color.green() & 0xff) << 8;
    cv |= (color.red() & 0xff) << 16;
    face.append(cv);
    }
  return face;
  }




//!
//! \brief matrixTop    Builds a matrix that translates to the center of the face and orients the z-axis along the normal
//! \param face         Source face
//! \param invertNormal Change normal direction to inverted
//! \return             Transformation matrix
//!
QMatrix4x4 Sd3drModel::matrixTop(const Sd3drFace &face, bool invertNormal )
  {
  invertNormal = !invertNormal;
  if( face.size() < 3 )
    return QMatrix4x4(); // Недостаточно точек
#define FLT_MAX 1e30
  // 1. Вычисляем центр полигона (среднее арифметическое всех вершин)
  // Вычисляем AABB (Axis-Aligned Bounding Box)
  QVector3D minPoint( FLT_MAX,  FLT_MAX,  FLT_MAX);
  QVector3D maxPoint(-FLT_MAX, -FLT_MAX, -FLT_MAX);

  for( int i : face ) {
    QVector3D p( mVertexList.at(i) );
    minPoint.setX( qMin(minPoint.x(), p.x()) );
    minPoint.setY( qMin(minPoint.y(), p.y()) );
    minPoint.setZ( qMin(minPoint.z(), p.z()) );
    maxPoint.setX( qMax(maxPoint.x(), p.x()) );
    maxPoint.setY( qMax(maxPoint.y(), p.y()) );
    maxPoint.setZ( qMax(maxPoint.z(), p.z()) );
    }

  // 2. Вычисляем нормаль к плоскости полигона
  // Используем первые три точки (они не коллинеарны, т.к. полигон выпуклый)
  QVector3D p0( mVertexList.at(face.at(0)) );
  QVector3D p1( mVertexList.at(face.at(1)) );
  QVector3D p2( mVertexList.at(face.at(2)) );
  QVector3D a = p1 - p0;
  QVector3D b = p2 - p0;
  QVector3D normal = QVector3D::crossProduct(a, b).normalized();

  // Вычисляем AABB центр
  QVector3D aabbCenter = (minPoint + maxPoint) / 2.0f;

  // Проецируем на плоскость полигона
  QVector3D center = aabbCenter - QVector3D::dotProduct(aabbCenter - p0, normal) * normal;

  // Инвертируем нормаль если нужно
  if( invertNormal )
    normal = -normal;

  // Инвертируем нормаль если нужно (для оси Z)

  // 3. Смещаем центр по нормали (или против нормали, если height отрицательный)
  // height > 0: смещение в сторону zAxis
  // height < 0: смещение в противоположную сторону

  // Вместо tempX используем tempY (глобальную ось Y)
  QVector3D tempY(0, 1, 0);  // Всегда стремимся к глобальной оси Y

  // Но нужно проверить, не параллельна ли нормаль оси Y
  if( qAbs(QVector3D::dotProduct(normal, tempY)) > 0.9999f ) {
    // Если нормаль почти параллельна Y, используем X как fallback
    tempY = QVector3D(1, 0, 0);
    }

  // Строим localY как проекцию tempY на плоскость
  QVector3D localY = QVector3D::crossProduct(normal, QVector3D::crossProduct(tempY, normal)).normalized();
  // Затем localX = crossProduct(localY, normal)
  QVector3D localX = QVector3D::crossProduct(localY, normal).normalized();

  // Строим матрицу
  QMatrix4x4 transform;
  transform(0, 0) = localX.x();
  transform(1, 0) = localX.y();
  transform(2, 0) = localX.z();
  transform(0, 1) = localY.x();
  transform(1, 1) = localY.y();
  transform(2, 1) = localY.z();
  transform(0, 2) = normal.x();
  transform(1, 2) = normal.y();
  transform(2, 2) = normal.z();
  transform(0, 3) = center.x();
  transform(1, 3) = center.y();
  transform(2, 3) = center.z();
  transform(3, 3) = 1;

  return transform;
  }



//!
//! \brief matrixNew Build complex matrix of rotation and offset
//! \param angX      Rotation angle around axis X in degree
//! \param angY      Rotation angle around axis Y in degree
//! \param angZ      Rotation angle around axis Z in degree
//! \param offX      Offset by X after rotation
//! \param offY      Offset by Y after rotation
//! \param offZ      Offset by Z after rotation
//! \return          Complex matrix
//!
QMatrix4x4 Sd3drModel::matrixNew(float angX, float angY, float angZ, float offX, float offY, float offZ)
  {
  QMatrix4x4 mat{};
  mat.translate( offX, offY, offZ );
  mat.rotate( angZ, 0, 0, 1.0 );
  mat.rotate( angY, 0, 1.0, 0 );
  mat.rotate( angX, 1.0, 0, 0 );
  return mat;
  }




//!
//! \brief matrixZ Build simple matrix with offset by Z
//! \param offZ    Offset by Z
//! \return        Translation matrix
//!
QMatrix4x4 Sd3drModel::matrixZ(float offZ)
  {
  QMatrix4x4 mat{};
  mat.translate( 0, 0, offZ );
  return mat;
  }





//!
//! \brief solidBox Builds a rectangular box, optionally without bottom
//! \param lenght   Box length (X-axis)
//! \param width    Box width (Y-axis)
//! \param height   Box height (Z-axis)
//! \param addBot   If true, adds bottom face; if false, bottom face is omitted
//! \return         List of faces forming the solid
//!
Sd3drFaceList Sd3drModel::solidBox(float lenght, float width, float height, bool addBot)
  {
  return solidNew( flatRectangle( lenght, width ), height, addBot );
  }





//!
//! \brief solidBoxWithCone Builds a rectangular box with a cone on top, optionally without bottom
//! \param lenght           Box length (X-axis)
//! \param width            Box width (Y-axis)
//! \param height           Box height (Z-axis)
//! \param coneHeight       Height of the cone
//! \param map              Transformation matrix
//! \param addBot           If true, adds bottom face; if false, bottom face is omitted
//! \return                 List of faces forming the solid
//!
Sd3drFaceList Sd3drModel::solidBoxWithCone(float lenght, float width, float height, float coneHeight, bool addBot)
  {
  return solidAddCone( solidBox( lenght, width, height, addBot ), coneHeight );
  }




//!
//! \brief solidBoxBevel Builds a rectangular box with beveled vertical edges, optionally without bottom
//! \param lenght          Box length (X-axis)
//! \param width           Box width (Y-axis)
//! \param height          Box height (Z-axis)
//! \param bevelSize       Size of the bevel
//! \param bevelCount      Number of bevel segments
//! \param map             Transformation matrix
//! \param addBot          If true, adds bottom face; if false, bottom face is omitted
//! \return                List of faces forming the solid
//!
Sd3drFaceList Sd3drModel::solidBoxBevel(float lenght, float width, float height, float bevelSize, float bevelCount, bool addBot)
  {
  return solidNew( flatRectangleBevel( lenght, width, bevelSize, bevelCount ), height, addBot );
  }




//!
//! \brief solidBoxRound Builds a rectangular box with rounded vertical edges, optionally without bottom
//! \param lenght        Box length (X-axis)
//! \param width         Box width (Y-axis)
//! \param height        Box height (Z-axis)
//! \param roundRadius   Radius of the rounded corners
//! \param roundCount    Number of rounding segments
//! \param map           Transformation matrix
//! \param addBot        If true, adds bottom face; if false, bottom face is omitted
//! \return              List of faces forming the solid
//!
Sd3drFaceList Sd3drModel::solidBoxRound(float lenght, float width, float height, float roundRadius, float roundCount, bool addBot)
  {
  return solidNew( flatRectangleRound( lenght, width, roundRadius, 360 / 30, roundCount ), height, addBot );
  }




//!
//! \brief solidCylinder Builds a cylinder, optionally without bottom
//! \param radius        Cylinder radius
//! \param height        Cylinder height
//! \param map           Transformation matrix
//! \param addBot        If true, adds bottom face; if false, bottom face is omitted
//! \return              List of faces forming the solid
//!
Sd3drFaceList Sd3drModel::solidCylinder(float radius, float height, bool addBot)
  {
  return solidNew( flatCircle( radius ), height, addBot );
  }





//!
//! \brief solidCylinderWithCone Builds a cylinder with a cone on top, optionally without bottom
//! \param radius                Cylinder radius
//! \param height                Cylinder height
//! \param coneHeight            Height of the cone
//! \param map                   Transformation matrix
//! \param addBot                If true, adds bottom face; if false, bottom face is omitted
//! \return                      List of faces forming the solid
//!
Sd3drFaceList Sd3drModel::solidCylinderWithCone(float radius, float height, float coneHeight, bool addBot)
  {
  return solidAddCone( solidCylinder( radius, height, addBot ), coneHeight );
  }





//!
//! \brief solidPlygedronInner Builds a regular polygon prism by inscribed circle radius
//! \param radius              Radius of the inscribed circle
//! \param height              Prism height
//! \param sideCount           Number of sides
//! \param map                 Transformation matrix
//! \param addBot              If true, adds bottom face; if false, bottom face is omitted
//! \return                    List of faces forming the solid
//!
Sd3drFaceList Sd3drModel::solidPlygedronInner(float radius, float height, float sideCount, bool addBot)
  {
  return solidNew( flatPlygedronInner( radius, sideCount ), height, addBot );
  }




//!
//! \brief solidPlygedronOuter Builds a regular polygon prism by circumscribed circle radius
//! \param radius              Radius of the circumscribed circle
//! \param height              Prism height
//! \param sideCount           Number of sides
//! \param map                 Transformation matrix
//! \param addBot              If true, adds bottom face; if false, bottom face is omitted
//! \return                    List of faces forming the solid
//!
Sd3drFaceList Sd3drModel::solidPlygedronOuter(float radius, float height, float sideCount, bool addBot)
  {
  return solidNew( flatPlygedronOuter( radius, sideCount ), height, addBot );
  }





//!
//! \brief solidTrapezoid Builds a trapezoid in the XY plane
//! \param lenghtTop      Length of the top edge
//! \param lenghtBot      Length of the bottom edge
//! \param width          Width of the trapezoid
//! \param height         Height of the trapezoid (extrusion)
//! \param map            Transformation matrix
//! \param addBot         If true, adds bottom face; if false, bottom face is omitted
//! \return               List of faces forming the solid
//!
Sd3drFaceList Sd3drModel::solidTrapezoid(float lenghtTop, float lenghtBot, float width, float height, bool addBot)
  {
  return solidNew( flatTrapezoid( lenghtTop, lenghtBot, width ), height, addBot );
  }





//!
//! \brief solidTrapezoidRound Builds a trapezoid in the XY plane with rounded vertical edges
//! \param lenghtTop           Length of the top edge
//! \param lenghtBot           Length of the bottom edge
//! \param width               Width of the trapezoid
//! \param height              Height of the trapezoid (extrusion)
//! \param roundRadius         Radius of the rounded corners
//! \param map                 Transformation matrix
//! \param addBot              If true, adds bottom face; if false, bottom face is omitted
//! \return                    List of faces forming the solid
//!
Sd3drFaceList Sd3drModel::solidTrapezoidRound(float lenghtTop, float lenghtBot, float width, float height, float roundRadius, bool addBot)
  {
  return solidNew( flatTrapezoidRound( lenghtTop, lenghtBot, width, roundRadius ), height, addBot );
  }




Sd3drFace reverse( Sd3drFace face )
  {
  std::reverse( face.begin(), face.end() );
  return face;
  }




//!
//! \brief solidRectPrizm Builds rectangular prizm with some floors. Each floor can have different size, height and center offset
//! \param lenght         Bottom lenght
//! \param width          Bottom width
//! \param floors         Each floor description (5 values): lenght, width, offset x, offset y, height
//! \param map            Transformation matrix
//! \param addBot         If true, adds bottom face; if false, bottom face is omitted
//! \return               List of faces forming the solid
//!
Sd3drFaceList Sd3drModel::solidRectPrizm(float lenght, float width, QList<float> floors, bool addBot)
  {
  Sd3drFaceList list;
  Sd3drFace faceBot = flatToFace( flatRectangle(lenght,width), 0 );
  if( addBot )
    list.append( reverse(faceBot) );

  float floorHeight = 0.0;
  for( int floorIndex = 0; floorIndex * 5 < floors.size(); ++floorIndex ) {
    float floorLenght  = floors.at( floorIndex * 5 + 0 );
    float floorWidth   = floors.at( floorIndex * 5 + 1 );
    float floorOffsetX = floors.at( floorIndex * 5 + 2 );
    float floorOffsetY = floors.at( floorIndex * 5 + 3 );
    floorHeight  += floors.at( floorIndex * 5 + 4 );

    Sd2dRegion floorRegion( flatOffset( flatRectangle(floorLenght,floorWidth), floorOffsetX, floorOffsetY )  );
    Sd3drFace floorFace( flatToFace( floorRegion, floorHeight ) );
    list.append( faceListWall( faceBot, floorFace, true ) );
    faceBot = floorFace;
    }

  //Append top
  list.append( faceBot );
  return list;
  }




Sd3drFaceList Sd3drModel::solid(const Sd3drFace &face, float height, const QMatrix4x4 &m, bool addBot, QColor color)
  {
  Sd3drFaceList list;
  if( color.isValid() )
    list.append( faceColor(color) );
  if( isZero(height) ) {
    //Append bottom if need
    if( addBot )
      list.append( reverse(face) );
    //Next operations remove this face
    list.append( face );
    }
  else {
    // Получаем локальную ось Z из матрицы (нормаль в мировых координатах)
    QVector3D localZ(m(0, 2), m(1, 2), m(2, 2));
    // Смещаем точку вдоль этой оси
    localZ *= height;

    //Build top face by offset each point from bottom face on vector localZ
    Sd3drFace top( faceDuplicateOffset( face, localZ ) );

    //Append bottom if need
    if( addBot )
      list.append( reverse(face) );

    //Build wall
    list.append( faceListWall( face, top, true ) );

    //Append top
    list.append( top );
    }
  return list;
  }



Sd3drFaceList Sd3drModel::solidNew(const Sd2dRegion &r, float height, bool addBot)
  {
  return solidNewColor( r, height, addBot, QColor{} );
  }



Sd3drFaceList Sd3drModel::solidNewColor(const Sd2dRegion &r, float height, bool addBot, QColor color)
  {
  QMatrix4x4 m;
  return solid( faceFromFlat( r, m), height, m, addBot, color );
  }







Sd3drFaceList Sd3drModel::solidTube(const Sd2dRegion &rOut, float thickness, float height, bool addBot)
  {
  return solidTubeColor( rOut, thickness, height, addBot, QColor{} );
  }

Sd3drFaceList Sd3drModel::solidTubeColor(const Sd2dRegion &rOut, float thickness, float height, bool addBot, QColor color)
  {
  QMatrix4x4 m;
  Sd3drFace faceOut = faceFromFlat( rOut, m );
  Sd3drFace faceIn  = faceFromFlat( flatEquidistant( rOut, thickness ), m );
  // Получаем локальную ось Z из матрицы (нормаль в мировых координатах)
  QVector3D localZ(m(0, 2), m(1, 2), m(2, 2));
  // Смещаем точку вдоль этой оси
  localZ *= height;

  //Build top face by offset each point from bottom face on vector localZ
  Sd3drFace topOut( faceDuplicateOffset( faceOut, localZ ) );
  Sd3drFace topIn( faceDuplicateOffset( faceIn, localZ ) );

  Sd3drFaceList list;
  if( color.isValid() )
    list.append( faceColor(color) );
  //Append bottom if need
  if( addBot )
    list.append( faceListWall( reverse(faceOut), reverse(faceIn), true ) );

  //Build wall
  list.append( faceListWall( faceOut, topOut, true ) );
  list.append( faceListWall( faceIn, topIn, true ) );

  //Append top
  list.append( faceListWall( topOut, topIn, true ) );
  return list;
  }




Sd3drFaceList Sd3drModel::solidTubeDif(const Sd2dRegion &rOut, const Sd2dRegion &rIn, float height, bool addBot)
  {
  return solidTubeDifColor( rOut, rIn, height, addBot, QColor{} );
  }

Sd3drFaceList Sd3drModel::solidTubeDifColor(const Sd2dRegion &rOut, const Sd2dRegion &rIn, float height, bool addBot, QColor color)
  {
  QMatrix4x4 m;
  Sd3drFace faceOut = faceFromFlat( rOut, m );
  Sd3drFace faceIn  = faceFromFlat( rIn, m );
  // Получаем локальную ось Z из матрицы (нормаль в мировых координатах)
  QVector3D localZ(m(0, 2), m(1, 2), m(2, 2));
  // Смещаем точку вдоль этой оси
  localZ *= height;

  //Build top face by offset each point from bottom face on vector localZ
  Sd3drFace topOut( faceDuplicateOffset( faceOut, localZ ) );
  Sd3drFace topIn( faceDuplicateOffset( faceIn, localZ ) );

  Sd3drFaceList list;
  if( color.isValid() )
    list.append( faceColor(color) );
  //Append bottom if need
  if( addBot )
    list.append( ring( rOut, faceOut, rIn, faceIn ) );

  //Build wall
  list.append( faceListWall( faceOut, topOut, true ) );
  list.append( faceListWall( faceIn, topIn, true ) );

  //Append top
  list.append( ring( rOut, topOut, rIn, topIn ) );
  return list;
  }



Sd3drFaceList Sd3drModel::solidBlind(const Sd2dRegion &rOut, float thickness, float height, float depth, bool addBot)
  {
  return solidBlindColor( rOut, thickness, height, depth, addBot, QColor{} );
  }

Sd3drFaceList Sd3drModel::solidBlindColor(const Sd2dRegion &rOut, float thickness, float height, float depth, bool addBot, QColor color)
  {
  return solidBlindDifColor( rOut, flatEquidistant( rOut, thickness ), height, depth, addBot, color );
  }








Sd3drFaceList Sd3drModel::solidBlindDif(const Sd2dRegion &rOut, const Sd2dRegion &rIn, float height, float depth, bool addBot)
  {
  return solidBlindDifColor( rOut, rIn, height, depth, addBot, QColor{} );
  }

Sd3drFaceList Sd3drModel::solidBlindDifColor(const Sd2dRegion &rOut, const Sd2dRegion &rIn, float height, float depth, bool addBot, QColor color)
  {
  QMatrix4x4 m;
  Sd3drFace faceOut = faceFromFlat( rOut, m );
  Sd3drFace faceIn  = faceFromFlat( rIn, m, height - depth );

  // Получаем локальную ось Z из матрицы (нормаль в мировых координатах)
  QVector3D localZOut(m(0, 2), m(1, 2), m(2, 2));
  QVector3D localZIn(localZOut);
  // Смещаем точку вдоль этой оси
  localZOut *= height;
  localZIn  *= depth;

  //Build top face by offset each point from bottom face on vector localZ
  Sd3drFace topOut( faceDuplicateOffset( faceOut, localZOut ) );
  Sd3drFace topIn( faceDuplicateOffset( faceIn, localZIn ) );

  Sd3drFaceList list;
  if( color.isValid() )
    list.append( faceColor(color) );
  //Append bottom if need
  if( addBot )
    list.append( faceOut );


  //Build wall
  list.append( faceListWall( faceOut, topOut, true ) );
  list.append( faceListWall( faceIn, topIn, true ) );

  //Append top
  list.append( ring( rOut, topOut, rIn, topIn ) );

  list.append( faceIn );
  return list;
  }




//!
//! \brief solidAddCone Adds a cone to the solid
//! \param faceList     Existing solid face list (top face will be removed)
//! \param coneHeight   Height of the cone
//! \return             New solid face list with the cone added
//!
Sd3drFaceList Sd3drModel::solidAddCone(const Sd3drFaceList &faceList, float coneHeight)
  {
  return solidAddConeColor( faceList, coneHeight, 0, 0, QColor{} );
  }





//!
//! \brief solidAddConeColor Adds a cone to the solid
//! \param faceList          Existing solid face list (top face will be removed)
//! \param coneHeight        Height of the cone
//! \param offx              Apex offset x
//! \param offy              Apex offset y
//! \param color             Cone color
//! \return                  New solid face list with the cone added
//!
Sd3drFaceList Sd3drModel::solidAddConeColor(const Sd3drFaceList &faceList, float coneHeight, float offx, float offy, QColor color)
  {
  Sd3drFaceList list(faceList);
  if( list.isEmpty() )
    list.append( solidCylinder( 1, 0.2, true ) );
  Sd3drFace topFace( list.takeLast() );
  QMatrix4x4 map = matrixTop( topFace );
  int top = vertexAppend( map.map( QVector3D{offx,offy,coneHeight} ) );
  if( color.isValid() )
    list.append( faceColor(color) );
  for( int i = 0; i < topFace.count(); ++i ) {
    int nextIndex = (i + 1) % topFace.count();
    list.append( { topFace.at(i), topFace.at(nextIndex), top });
    }
  return list;
  }




//!
//! \brief solidAddRoofRound Creates a rounded top face (roof)
//! \param faceList          Existing solid face list (top face will be removed)
//! \param roundRadius       Radius of the rounding, >0 - convex, <0 - concave
//! \param great             true - top size greater than source, false = top size less than source
//! \return                  New solid face list with rounded roof
//!
Sd3drFaceList Sd3drModel::solidAddRoofRound(const Sd3drFaceList &faceList, float roundRadius, bool great)
  {
  Sd3drFaceList list(faceList);
  if( list.isEmpty() )
    list.append( solidCylinder( 1, 0.2, true ) );
  Sd3drFace top(list.takeLast());
  QMatrix4x4 m( matrixTop( top, 0 ) );
  Sd2dRegion r1( flatFromFace( top, m ) );
  float step = M_PI/2.0/16;
  bool doSin;
  float mul = great ? -1.0 : 1.0;
  if( roundRadius > 0 )
    doSin = great;
  else {
    roundRadius = -roundRadius;
    doSin = !great;
    }
  if( doSin ) {
    for( float ang = step; ang < M_PI/2.0; ang += step ) {
      float dist = roundRadius * sin( ang );
      float h = roundRadius - roundRadius * cos( ang );
      Sd2dRegion r2( flatEquidistant( r1, dist * mul ) );
      Sd3drFace roof( faceFromFlat( r2, m, h ) );
      list.append( faceListWall( top, roof, true ) );
      top = roof;
      }
    }
  else {
    for( float ang = step; ang < M_PI/2.0; ang += step ) {
      float dist = roundRadius - roundRadius * cos( ang );
      float h = roundRadius * sin( ang );
      Sd2dRegion r2( flatEquidistant( r1, dist * mul ) );
      Sd3drFace roof( faceFromFlat( r2, m, h ) );
      list.append( faceListWall( top, roof, true ) );
      top = roof;
      }
    }
  list.append( top );
  return list;
  }






//!
//! \brief solidAddRoofBevel Creates a beveled top face (roof)
//! \param faceList          Existing solid face list (top face will be removed)
//! \param bevelSize         Size of the bevel
//! \return                  New solid face list with beveled roof
//!
Sd3drFaceList Sd3drModel::solidAddRoofBevel(const Sd3drFaceList &faceList, float bevelSize)
  {
  return solidAddRoofBevelColor( faceList, bevelSize, QColor{} );
  }



Sd3drFaceList Sd3drModel::solidAddRoofBevelColor(const Sd3drFaceList &faceList, float bevelSize, QColor color)
  {
  Sd3drFaceList list(faceList);
  if( list.isEmpty() )
    list.append( solidCylinder( 1, 0.2, true ) );
  Sd3drFace top(list.takeLast());
  QMatrix4x4 m( matrixTop( top, 0 ) );
  Sd2dRegion r1( flatFromFace( top, m ) );
  Sd2dRegion r2( flatEquidistant( r1, bevelSize ) );
  Sd3drFace roof( faceFromFlat( r2, m, fabs(bevelSize) ) );
  if( color.isValid() )
    list.append( faceColor(color) );
  list.append( faceListWall( top, roof, true ) );
  list.append( roof );
  return list;
  }







//!
//! \brief solidAdd Adds an extrusion with the same profile as the top face
//! \param faceList Existing solid face list (top face will be removed)
//! \param height   Extrusion height
//! \return         New solid face list with the added extrusion
//!
Sd3drFaceList Sd3drModel::solidAdd(const Sd3drFaceList &faceList, float thickness, float height, float offset)
  {
  return solidAddColor( faceList, thickness, height, offset, QColor{} );
  }


Sd3drFaceList Sd3drModel::solidAddColor(const Sd3drFaceList &faceList, float thickness, float height, float offset, QColor color)
  {
  Sd3drFaceList list(faceList);
  if( list.isEmpty() )
    list.append( solidCylinder( 1, 0.2, true ) );
  Sd3drFace face(list.takeLast());
  QMatrix4x4 map = matrixTop( face, 0 );

  if( color.isValid() )
    list.append( faceColor(color) );
  if( isZero(thickness) )
    list.append( solid( face, height + offset, map, false, QColor{} ) );
  else {
    Sd2dRegion srcR = flatFromFace( face, map );
    Sd2dRegion inR  = flatEquidistant( srcR, thickness );
    Sd3drFace faceIn = faceFromFlat( inR, map, offset );
    Sd3drFace faceTop = faceFromFlat( inR, map, offset + height );
    list.append( faceListWall( face, faceIn, true ) );
    list.append( faceListWall( faceIn, faceTop, true ) );
    list.append( faceTop );
    }
  return list;
  }





//!
//! \brief solidAddDif   Adds an extrusion with r profile at the top face
//! \param faceList      Sorce face list with top face as base for extrusion
//! \param r             Extrusion profile
//! \param height        Extrusion height
//! \param offset        Offset base extrusion from top face of source
//! \return              New solid face list with the added extrusion
//!
Sd3drFaceList Sd3drModel::solidAddDif(const Sd3drFaceList &faceList, const Sd2dRegion &r, float height, float offset)
  {
  return solidAddDifColor( faceList, r, height, offset, QColor{} );
  }

Sd3drFaceList Sd3drModel::solidAddDifColor(const Sd3drFaceList &faceList, const Sd2dRegion &r, float height, float offset, QColor color)
  {
  Sd3drFaceList list(faceList);
  if( list.isEmpty() )
    list.append( solidCylinder( 1, 0.2, true ) );
  Sd3drFace top(list.takeLast());
  QMatrix4x4 topMatrix( matrixTop( top, 0 ) );
  Sd2dRegion topRegion( flatFromFace( top, topMatrix ) );
  //QMatrix4x4 baseMatrix( matrixTop( top, offset ) );
  Sd3drFace base( faceFromFlat( r, topMatrix, offset ) );
  list.append( ring( topRegion, top, r, base ) );
  Sd3drFace up( faceFromFlat( r, topMatrix, height + offset ) );
  if( color.isValid() )
    list.append( faceColor(color) );
  list.append( faceListWall( base, up, true ) );
  list.append( up );
  return list;
  }




//!
//! \brief solidAddBlindArray Append to top face array of blind holes
//! \param faceList           Face from witch need to taken top face
//! \param r                  Hole profile
//! \param depth              Hole depth
//! \param rowCount           Count of hole rows
//! \param rowDistance        Distance between rows
//! \param rowDescr           Rows description. For each row: hole distance, row hole count, horizontal offset, vertical offset
//! \return                   New solid face list with the added extrusion
//!
Sd3drFaceList Sd3drModel::solidAddBlindArray(const Sd3drFaceList &faceList, const Sd2dRegion &r, float depth, float rowDistance, const QList<float> &rowDescr)
  {
  return solidAddBlindArrayColor( faceList, r, depth, rowDistance, rowDescr, QColor{} );
  }

Sd3drFaceList Sd3drModel::solidAddBlindArrayColor(const Sd3drFaceList &faceList, const Sd2dRegion &r, float depth, float rowDistance, const QList<float> &rowDescr, QColor color)
  {
  Sd3drFaceList list(faceList);
  if( list.isEmpty() || rowDescr.size() < 4 )
    list.append( solidCylinder( 1, 0.2, true ) );
  Sd3drFace top(list.takeLast());
  QMatrix4x4 topMatrix( matrixTop( top, 0 ) );
  Sd2dRegion topRegion( flatFromFace( top, topMatrix ) );

  QList<Sd2dRegion> holeRegionList;
  Sd3drFaceList holes;

  Sd3drPointLinkList pointPool;
  QList<Sd3drPointLinkPtr> srcList;
  QList<Sd3drPointLinkPtr> srcHole;
  srcList.append( pointPool.addRegion( top, topRegion, true ) );

  //Fill hole array
  for( int rowIndex = 0; rowIndex * 4 < rowDescr.size(); ++rowIndex ) {
    float holeHorzDistance = qBound( -1000.0, rowDescr.at( rowIndex * 4 + 0 ), 1000.0 );
    float holeCount        = qBound( 1.0, rowDescr.at( rowIndex * 4 + 1 ), 200.0 );
    float horzOffset       = qBound( -1000.0, rowDescr.at( rowIndex * 4 + 2 ), 1000.0 );
    float vertOffset       = qBound( -1000.0, rowDescr.at( rowIndex * 4 + 3 ), 1000.0 );

    float offX = (holeCount - 1.0) * holeHorzDistance / -2.0 + horzOffset;
    float offY = (rowDescr.size()/4 - 1) * rowDistance / -2.0 + vertOffset + rowIndex * rowDistance;
    for( int holeIndex = 0; holeIndex < holeCount; ++holeIndex ) {
      //Make hole region
      Sd2dRegion hole( flatOffset( r, offX, offY ) );
      //Append region to hole list
      holeRegionList.append( hole );

      //Build top face
      Sd3drFace holeTopFace = faceFromFlat( hole, topMatrix );
      Sd3drFace holeBotFace = faceFromFlat( hole, topMatrix, -depth );
      holes.append( faceListWall( holeTopFace, holeBotFace, true ) );
      holes.append( holeBotFace );

      srcHole.append( pointPool.addRegion( holeTopFace, hole, false ) );
      }
    }

  //Build top face with holes
  for( int faceIndex = 0; faceIndex < srcList.count(); faceIndex++ ) {
    Sd3drPointLinkPtr face = srcList.at(faceIndex);
    while( !face->isTriangle() ) {
      face = face->lessLeft();
      SdTriangle t;
      t.mLeftA = face->mPoint;
      t.mNextB = face->mNext->mPoint;
      t.mPrevC = face->mPrev->mPoint;
      t.prepare();

      //Test all remain points of region and holes
      Sd3drPointLinkPtr inner = nullptr;
      for( Sd3drPointLinkPtr ptr = face->mNext->mNext; ptr != face->mPrev; ptr = ptr->mNext ) {
        if( t.isPointInside( ptr->mPoint ) ) {
          if( inner == nullptr || ptr->isLeft( inner ) )
            inner = ptr;
          }
        }
      //Test holes
      int usedHole = -1;
      for( int holeIndex = 0; holeIndex < srcHole.count(); holeIndex++ ) {
        //Test one hole
        Sd3drPointLinkPtr hole = srcHole.at(holeIndex);
        Sd3drPointLinkPtr ptr = hole;
        do {
          if( t.isPointInside( ptr->mPoint ) ) {
            if( inner == nullptr || ptr->isLeft( inner ) ) {
              inner = ptr;
              usedHole = holeIndex;
              }
            }
          ptr = ptr->mNext;
          }
        while( ptr != hole );
        }
      if( inner != nullptr ) {
        //Divide source region into two regions or union source with hole
        face->splitRegion( inner, &pointPool );
        if( usedHole >= 0 )
          srcHole.removeAt(usedHole);
        else
          srcList.append( inner );
        }
      else {
        //Remove triangle from face
        list.append( face->triangle() );
        face = face->remove();
        }
      }
    list.append( face->triangle() );
    }

  //Append color for holes if there
  if( color.isValid() )
    list.append( faceColor(color) );

  //Append holes
  list.append( holes );

  return list;

  }





//!
//! \brief solidAddCurveVector Adds a curved bend of the profile with given radius along the specified vector
//! \param faceList            Existing solid face list (top face will be removed)
//! \param radius              Bend radius
//! \param sideCount           Number of segments for the curve
//! \param x                   X component of the direction vector
//! \param y                   Y component of the direction vector
//! \param z                   Z component of the direction vector
//! \return                    New solid face list with the curved bend
//!
//! The initial direction is considered to be the normal of the original profile
Sd3drFaceList Sd3drModel::solidAddCurveVector(const Sd3drFaceList &faceList, float radius, float sideCount, float x, float y, float z)
  {
  Sd3drFaceList list(faceList);
  if( list.isEmpty() )
    list.append( solidCylinder( 1, 0.2, true ) );
  Sd3drFace top(list.takeLast());
  QMatrix4x4 map( matrixTop( top, 0.0 )  );
  // 1. Переводим регион в локальные координаты
  QMatrix4x4 invMap = map.inverted();
  QList<QVector3D> localRegion;
  for( int i : std::as_const(top) )
    localRegion.append( invMap.map(vertex(i)) );

  // 2. Локальная нормаль (ось Z)
  QVector3D localNormal(0, 0, 1);

  // 3. Вычисляем угол между нормалью и вектором поворота
  QVector3D localRotationVector( x, y, z );
  QVector3D rotationDir = localRotationVector.normalized();
  float dot = QVector3D::dotProduct(localNormal, rotationDir);
  dot = qBound(-1.0f, dot, 1.0f);
  float totalAngleRad = std::acos(dot);
  float totalAngleDeg = totalAngleRad * 180.0f / M_PI;

  qDebug() << "Полный угол поворота:" << totalAngleDeg << "°";

  // 4. Определяем направление смещения в плоскости XY (проекция вектора поворота)
  QVector3D projectionXY(localRotationVector.x(), localRotationVector.y(), 0);
  if( projectionXY.length() < 0.0001f ) {
    qWarning() << "Вектор поворота перпендикулярен плоскости XY, смещение не определено";
    return list;
    }
  QVector3D offsetDir = projectionXY.normalized();

  // 5. Центр поворота в локальных координатах (смещение в плоскости XY)
  QVector3D localPivot = offsetDir * radius;

  // 6. Ось поворота: перпендикулярна плоскости, образованной нормалью и вектором поворота
  // Ось = нормаль × вектор_поворота (или нормализованная)
  QVector3D localRotationAxis = QVector3D::crossProduct(localNormal, rotationDir).normalized();

  // Если ось оказалась нулевой (векторы коллинеарны), поворот не требуется
  if( localRotationAxis.length() < 0.0001f ) {
    qDebug() << "Вектор поворота коллинеарен нормали, поворота нет";
    return list;
    }

  // 7. Угловой шаг
  float stepAngleRad = totalAngleRad / sideCount;

  // 8. Для каждого шага создаём матрицу поворота
  for( int step = 1; step <= sideCount; ++step ) {
    float currentAngleRad = stepAngleRad * step;
    float currentAngleDeg = currentAngleRad * 180.0f / M_PI;

    // Создаём матрицу поворота в локальных координатах
    QMatrix4x4 localRotationMatrix;

    // 3.Возвращаем центр поворота обратно
    localRotationMatrix.translate(localPivot);

    // 2.Поворачиваем вокруг оси
    localRotationMatrix.rotate(currentAngleDeg, localRotationAxis.x(), localRotationAxis.y(), localRotationAxis.z());

    // 1.Смещаем центр поворота в начало координат
    localRotationMatrix.translate(-localPivot);

    // Применяем поворот к региону
    Sd3drFace rotated;
    for(const QVector3D& p : localRegion)
      rotated.append( vertexAppend( map.map(localRotationMatrix.map(p) ) ) );

    list.append( faceListWall( top, rotated, true ) );
    top = rotated;
    }

  //Append top face
  list.append( top );

  return list;
  }





//!
//! \brief solidAddCurveXZ Adds a curved bend of the profile with given radius and angle around the Y axis
//! \param faceList        Existing solid face list (top face will be removed)
//! \param radius          Bend radius
//! \param curveAngle      Bend angle in degrees around axiz Y
//! \param sideCount       Number of segments for the curve
//! \return                New solid face list with the curved bend
//!
//! The initial direction is considered to be the normal of the original profile
Sd3drFaceList Sd3drModel::solidAddCurveXZ(const Sd3drFaceList &faceList, float radius, float sideCount, float curveAngle)
  {
  QMatrix4x4 rotationMatrix;
  rotationMatrix.rotate(curveAngle, 0, 1, 0);
  QVector3D r( rotationMatrix.map(QVector3D(0,0,1)) );
  return solidAddCurveVector( faceList, radius, sideCount, r.x(), r.y(), r.z() );
  }





//!
//! \brief solidAddHole Adds a hole to the top face with same profile as top face
//! \param faceList     Existing solid face list (top face will be removed)
//! \param thickness    Thickness of border
//! \param depth        Hole depth
//! \return             New solid face list with the hole added
//!
Sd3drFaceList Sd3drModel::solidAddHole(const Sd3drFaceList &faceList, float thickness, float depth)
  {
  return solidAddHoleColor( faceList, thickness, depth, QColor{} );
  }

Sd3drFaceList Sd3drModel::solidAddHoleColor(const Sd3drFaceList &faceList, float thickness, float depth, QColor color)
  {
  Sd3drFaceList list(faceList);
  if( list.isEmpty() )
    list.append( solidCylinder( 1, 0.2, true ) );
  Sd3drFace face(list.takeLast());
  QMatrix4x4 map( matrixTop( face, 0.0 )  );
  Sd2dRegion r = flatFromFace( face, map );
  Sd2dRegion rIn = flatEquidistant( r, thickness );
  Sd3drFace faceIn = faceFromFlat( rIn, map, 0 );
  list.append( faceListWall( face, faceIn, true ) );
  if( !isZero(depth) ) {
    if( color.isValid() )
      list.append( faceColor(color) );
    Sd3drFace depthIn = faceFromFlat( rIn, map, -depth );
    list.append( faceListWall( faceIn, depthIn, true ) );
    }
  return list;
  }




//!
//! \brief solidAddHoleDif Adds a hole to the top face with defferent profile
//! \param faceList        Existing solid face list (top face will be removed)
//! \param r               Hole profile
//! \param depth           Hole depth
//! \return                New solid face list with the hole added
//!
Sd3drFaceList Sd3drModel::solidAddHoleDif(const Sd3drFaceList &faceList, const Sd2dRegion &r, float depth)
  {
  return solidAddHoleDifColor( faceList, r, depth, QColor{} );
  }

Sd3drFaceList Sd3drModel::solidAddHoleDifColor(const Sd3drFaceList &faceList, const Sd2dRegion &r, float depth, QColor color)
  {
  Sd3drFaceList list(faceList);
  if( list.isEmpty() )
    list.append( solidCylinder( 1, 0.2, true ) );
  Sd3drFace face(list.takeLast());
  QMatrix4x4 map( matrixTop( face, 0.0 )  );
  Sd2dRegion rOut = flatFromFace( face, map );
  Sd3drFace faceIn = faceFromFlat( r, map, 0 );
  list.append( ring( rOut, face, r, faceIn ) );
  if( !isZero(depth) ) {
    if( color.isValid() )
      list.append( faceColor(color) );
    Sd3drFace depthIn = faceFromFlat( r, map, -depth );
    list.append( faceListWall( faceIn, depthIn, true ) );
    }
  return list;
  }














//!
//! \brief faceListExtrude Extrudes model from source face in direction specified by map
//! \param face            Source face of model
//! \param map             Direction of extruding
//! \return                Solid model: floor, roof and walls
//!
Sd3drFaceList Sd3drModel::faceListExtrude(const Sd3drFace &face, const QMatrix4x4 &map, bool addBot, bool addTop)
  {
  Sd3drFaceList faceList;
  if( addBot )
    //Append bottom
    faceList.append( face );

  //Create top
  Sd3drFace other = faceDuplicate( face, map );

  //Append walls
  faceList.append( faceListWall( face, other, true ) );

  if( addTop )
    //Append top
    faceList.append( other );

  return faceList;
  }






//!
//! \brief faceListExtrudeShift Extrudes model from region in the direction of the normal vector with
//!                             specified shift amount color faces.
//!                             First face is bottom (begin of vector), last face is top (end of vector),
//!                             Middle faces are walls.
//! \param face                 Region of bottom of model
//! \param shift                Shift amount of extrude
//! \param addBot               Add bottom face to result face list
//! \param addTop               Add top face to result face list
//! \return                     Solid model extruded from region in the direction of the normal vector
//!
Sd3drFaceList Sd3drModel::faceListExtrudeShift(const Sd3drFace &face, float shift, bool addBot, bool addTop )
  {
  return faceListExtrude( face, matrixShift( face, shift ), addBot, addTop );
  }



//!
//! \brief faceListBodyBeveled  Builds beveled body of part
//! \param bodyLenght           Full body lenght (X)
//! \param bodyWidth            Full body width (Y)
//! \param bodyHeight           Full body height (H)
//! \param bevelFront           Bevel size in front of body
//! \param bevelSide            Bevel size in other sides of body
//! \param verticalHeight       Vertical walls height of body for pin placing
//! \return                     Model of beveled body of part
//!
Sd3drFaceList Sd3drModel::faceListBodyBeveled(float bodyLenght, float bodyWidth, float bodyHeight, float bevelFront, float bevelSide, float verticalHeight )
  {
  //Bottom rectangle
  Sd3drFace bottom = faceRectangle( bodyLenght, bodyWidth, QMatrix4x4() );

  float middleHeight = (bodyHeight - verticalHeight) / 2.0;
  Sd3drFace middleLow = faceTrapezoidZ( bottom, middleHeight, -bevelSide, -bevelSide, 0, 0 );

  Sd3drFace middleHigh = faceDuplicateShift( middleLow, verticalHeight );

  float deltaBevel = (bevelFront - bevelSide) / 2.0;
  Sd3drFace top = faceTrapezoidZ( bottom, bodyHeight, 0, deltaBevel, 0, deltaBevel );

  //Build walls
  Sd3drFaceList body;
  body.append( bottom );
  body.append( faceListWall( bottom, middleLow, true ) );
  body.append( faceListWall( middleLow, middleHigh, true ) );
  body.append( faceListWall( middleHigh, top, true ) );
  body.append( top );

  return body;
  }




//!
//! \brief faceListBox  Builds box model from its size
//! \param lenght       Lenght of box (x)
//! \param width        Width of box (y)
//! \param height       Height of box (z)
//! \param map          Mapping matrix for box
//! \return             Box model
//!
Sd3drFaceList Sd3drModel::faceListBox(float lenght, float width, float height, const QMatrix4x4 &map )
  {
  return faceListExtrudeShift( faceRectangle( lenght, width, map ), height );
  }



//!
//! \brief faceListCylinder  Builds cylinder model from its size
//! \param radius            Radius of circle of footing of cylinder
//! \param height            Height of cylinder
//! \return                  Cylinder model
//!
Sd3drFaceList Sd3drModel::faceListCylinder(float radius, float height, const QMatrix4x4 &map)
  {
  return faceListExtrudeShift( faceCircleSide( radius, qMax( 20, static_cast<int>(radius * 6.28) ), map ), height );
  }



//!
//! \brief sd3dModelHexagon Builds hexagonal box body of part
//! \param lenght           Lenght of box
//! \param topLenght        Top lenght excluding bevels
//! \param height           Height of box
//! \param width            Width of box
//! \param color            Color of faces of model
//! \return                 Model of hexagonal box body of part
//!
Sd3drFaceList Sd3drModel::faceListHexagon(float lenght, float topLenght, float height, float width)
  {
  //Profile of hexagon
  float bevelLenght = (lenght - topLenght) / 2.0;
  QVector3D v0( -topLenght / 2.0, -width / 2.0, 0 );
  QVector3D v1 = v0 + QVector3D( -bevelLenght, 0, height / 2.0 );
  QVector3D v2 = v1 + QVector3D(  bevelLenght, 0, height / 2.0 );
  QVector3D v3 = v2 + QVector3D(  topLenght, 0, 0 );
  QVector3D v4 = v3 + QVector3D(  bevelLenght, 0, -height / 2.0 );
  QVector3D v5 = v4 + QVector3D( -bevelLenght, 0, -height / 2.0 );
  Sd3drFace faceLeft;
  faceLeft << vertexAppend(v0) << vertexAppend(v1) << vertexAppend(v2) << vertexAppend(v3) << vertexAppend(v4) << vertexAppend(v5);
  return faceListExtrudeShift( faceLeft, width );
  }




Sd3drFaceList Sd3drModel::faceListCurveXZ(const Sd3drFace &face, float radius, float angleSrc, float angleDst, int sideCount, bool close )
  {
  Sd3drFaceList model;
  Sd3drFace first(face);
  Sd3drFace second;
  float angleStep = angleDst / sideCount;
  float angle = angleStep;
  for( int i = 1; i < sideCount; i++ ) {
    second = faceCurveXZ( face, radius, angleSrc, angle );
    model.append( faceListWall( first, second, close ) );
    angle += angleStep;
    first = second;
    }
  //Last walls
  second = faceCurveXZ( face, radius, angleSrc, angleDst );
  model.append( faceListWall( first, second, close ) );
  return model;
  }




Sd3drFaceList Sd3drModel::faceListPinCurveOne(const Sd3drFace &face, float firstLen, float middleLen, float radius, float angle, int sideCount)
  {
  Sd3drFaceList pin;
  Sd3drFace faceMiddleStart = faceDuplicateShift( face, firstLen );
  pin = faceListWall( face, faceMiddleStart, true );
  Sd3drFace faceMiddleStop = faceCurveXZ( faceMiddleStart, radius, 0, angle );
  pin.append( faceListCurveXZ( faceMiddleStart, radius, 0, angle, sideCount, true ) );
  Sd3drFace faceLast = faceDuplicateShift( faceMiddleStop, middleLen );
  pin.append( faceListWall( faceMiddleStop, faceLast, true ) );
  pin.append( faceLast );
  return pin;
  }




Sd3drFaceList Sd3drModel::faceListPinCurveTwo(const Sd3drFace &face, float firstLen, float middleLen, float lastLen, float radius, float angleFirst, float angleSecond, int sideCount)
  {
  Sd3drFaceList pin;
  Sd3drFace faceMiddleStart = faceDuplicateShift( face, firstLen );
  pin = faceListWall( face, faceMiddleStart, true );
  Sd3drFace faceMiddleStop = faceCurveXZ( faceMiddleStart, radius, 0, angleFirst );
  pin.append( faceListCurveXZ( faceMiddleStart, radius, 0, angleFirst, sideCount, true ) );
  Sd3drFace faceSecondStart = faceDuplicateShift( faceMiddleStop, middleLen );
  pin.append( faceListWall( faceMiddleStop, faceSecondStart, true ) );
  Sd3drFace faceSecondStop = faceCurveXZ( faceSecondStart, -radius, angleFirst, angleSecond );
  pin.append( faceListCurveXZ( faceSecondStart, -radius, angleFirst, angleSecond, sideCount, true ) );
  Sd3drFace faceLast = faceDuplicateShift( faceSecondStop, lastLen );
  pin.append( faceListWall( faceSecondStop, faceLast, true ) );
  pin.append( faceLast );
  return pin;
  }




Sd3drFaceList Sd3drModel::faceListPinTqfp(float width, float thickness, float fullLenght, float plateLenght, float height)
  {
  return faceListPinCurveTwo( faceRectangle( width, thickness, QMatrix4x4()), fullLenght - plateLenght, height, plateLenght, thickness, 90, -90, 30 );
  }




Sd3drFaceList Sd3drModel::faceListWallRound(const Sd3drFace &face1, const Sd3drFace &face2, float scaleX, float scaleY, float radius, float stepDegree)
  {
  if( stepDegree >= 90.0 )
    return faceListWall( face1, face2, true );

  Sd3drFaceList faceList;
  Sd3drFace first = face1;
  float scaleDX = scaleX - 1.0;
  float scaleDY = scaleY - 1.0;
  if( stepDegree < 0 ) {
    stepDegree = -stepDegree;
    for( float angleDegree = stepDegree; angleDegree < 90.0; angleDegree += stepDegree ) {
      //Convert degree to radians
      float angle = angleDegree * M_PI / 180.0;
      Sd3drFace second = faceDuplicateScale( face1, 1.0 + scaleDX * sin(angle), 1.0 + scaleDY * sin(angle), radius - radius * cos(angle) );
      faceList.append( faceListWall( first, second, true ) );
      first = second;
      }
    }
  else {
    for( float angleDegree = stepDegree; angleDegree < 90.0; angleDegree += stepDegree ) {
      //Convert degree to radians
      float angle = angleDegree * M_PI / 180.0;
      Sd3drFace second = faceDuplicateScale( face1, 1.0 + scaleDX * (1.0 - cos(angle)), 1.0 + scaleDY * (1.0 - cos(angle)), radius * sin(angle) );
      faceList.append( faceListWall( first, second, true ) );
      first = second;
      }
    }
  faceList.append( faceListWall( first, face2, true ) );
  return faceList;
  }




Sd3drFaceList Sd3drModel::faceListWallBevelXY(const Sd3drFace &face1, const Sd3drFace &face2, float radius, float stepDegree)
  {
  float sizex,sizey;
  faceSizeXY( face1, sizex, sizey );
  return faceListWallRound( face1, face2, (sizex + radius) / sizex, (sizey + radius) / sizey, radius, stepDegree );
  }





Sd3drFaceList Sd3drModel::faceListWallDoubleBevelXY(const Sd3drFace &face1, const Sd3drFace &face2, float radius1, float radius2, float stepDegree1, float stepDegree2, float height)
  {
  Sd3drFaceList walls;
  float midHeight = height - (fabs(radius1) + fabs(radius2));
  Sd3drFace mid0 = faceBevelXY( face1, radius1 );
  Sd3drFace mid1 = midHeight > 0 ? faceDuplicateShift( mid0, midHeight ) : mid0;
  float sizex,sizey;
  faceSizeXY( face1, sizex, sizey );
  walls.append( faceListWallRound( face1, mid0, (sizex + radius1) / sizex, (sizey + radius1) / sizey, fabs(radius1), stepDegree1 ) );
  if( midHeight > 0 )
    walls.append( faceListWall( mid0, mid1, true ) );
  faceSizeXY( mid1, sizex, sizey );
  walls.append( faceListWallRound( mid1, face2, (sizex + radius2) / sizex, (sizey + radius2) / sizey, fabs(radius2), stepDegree2 ) );
  return walls;
  }



Sd3drFaceList Sd3drModel::faceListRotation(const QList<float> &pairList, float angleStart, float angleStop, int sideCount, QMatrix4x4 transfer )
  {
  Sd3drFaceList faceList;

  //Build source face
  int count = pairList.count() / 2;
  if( count < 2 )
    return faceList;
  count <<= 1;
  QVector3D v;
  QList<QVector3D> pathSrc;
  for( int i = 0; i < count; i += 2 ) {
    QVector3D dv( pairList.at(i), 0, pairList.at(i+1) );
    v += dv;
    pathSrc.append( v );
    }

  //Build rotation wires
  QList< QList<QVector3D> > wires;
  if( sideCount < 2 ) sideCount = 2;
  float angleStep = (angleStop - angleStart) / static_cast<float>(sideCount);
  for( int i = 0; i <= sideCount; i++ ) {
    //Rotation matrix
    QMatrix4x4 mat;
    mat.rotate( angleStart, 0, 0, 1.0 );

    //Build next path
    QList<QVector3D> pathDst;
    pathDst.reserve( pathSrc.count() );
    for( auto v : pathSrc )
      pathDst.append( mat.map( v ) );

    //Append to dest wires
    wires.append( pathDst );

    //Next angle
    angleStart += angleStep;
    }

  //Convert wires to layer faces
  for( int layerIndex = 0; layerIndex < pathSrc.count(); layerIndex++ ) {
    Sd3drFace face;
    face.reserve( sideCount + 1 );
    for( int vertexIndex = 0; vertexIndex <= sideCount; vertexIndex++ ) {
      face.append( vertexAppend( transfer.map(wires.at(vertexIndex).at(layerIndex))) );
      }
    faceList.append( face );
    }
  return faceList;
  }




//!
//! \brief faceListWalls Builds walls on base layers, count of element each of them is equals each other
//! \param layers        List of faces throught which will be builds walls
//! \param close         If true then append wall with n-1 and 0 index vertex on each layer
//! \return              List of walls
//!
Sd3drFaceList Sd3drModel::faceListWalls(const Sd3drFaceList &layers, bool close)
  {
  Sd3drFaceList walls;
  for( int i = 1; i < layers.count(); i++ )
    walls.append( faceListWall( layers.at(i-1), layers.at(i), close ) );
  return walls;
  }








//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void Sd3drModel::json(SvJsonWriter3d &js) const
  {
  json3dRegion( js, QStringLiteral("Vertex"), mVertexList );
  js.jsonList( js, QStringLiteral("Instance"), mInstanceList );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void Sd3drModel::json(const SvJsonReader3d &js)
  {
  json3dRegion( js, QStringLiteral("Vertex"), mVertexList );
  js.jsonList( js, QStringLiteral("Instance"), mInstanceList );
  }




//!
//! \brief draw3d Draws model in 3d space
//! \param f      OpenGL functions
//! \param mat    Conversion matrix for model
//!
void Sd3drModel::draw3d(QOpenGLFunctions_2_0 *f, QMatrix4x4 mat) const
  {
  for( auto const &instance : mInstanceList )
    instance.draw( f, mVertexList, mat );
  }





//!
//! \brief volumeAdd Append volume of model to result volume
//! \param volume    Source and result volume
//!
void Sd3drModel::volumeAdd(QMatrix2x3 &volume) const
  {
  for( auto const &inst : std::as_const(mInstanceList) ) {
    inst.volumeAdd( volume, mVertexList );
    }
  }



void Sd3drModel::faceSizeXY(const Sd3drFace &face, float &sizex, float &sizey) const
  {
  QPointF p = point( face.at(0) );
  float minX = p.x();
  float maxX = p.x();
  float minY = p.y();
  float maxY = p.y();
  for( auto index : face ) {
    p = point( index );
    if( minX > p.x() ) minX = p.x();
    if( maxX < p.x() ) maxX = p.x();
    if( minY > p.y() ) minY = p.y();
    if( maxY < p.y() ) maxY = p.y();
    }

  sizex = maxX - minX;
  sizey = maxY - minY;
  }
