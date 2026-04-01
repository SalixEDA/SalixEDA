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
#ifndef SD3DMODEL_H
#define SD3DMODEL_H

#include "Sd3drInstance.h"

using Sd2dRegion = QList<QVector2D>;

class Sd3drModel
  {
    Sd3drRegion       mVertexList;   //!< 3d vertex list
    Sd3drInstanceList mInstanceList; //!< 3d instance list, each instance contains one or more copy of some body list
  public:

    Sd3ColorList  colorList() const;

    void          colorListSet( const Sd3ColorList &lst );

    //!
    //! \brief clear Clear model
    //!
    void          clear();

    static Sd2dRegion flatRectangle( float lenght, float width );

    static Sd2dRegion flatRectangleBevel( float lenght, float width, float bevelSize, int count );

    static Sd2dRegion flatRectangleRound( float lenght, float width, float radius, float stepDegree, int count );

    static Sd2dRegion flatEllipse(float radiusx, float radiusy, int stepDegree );

    static Sd2dRegion flatCircle( float radius );

    static Sd2dRegion flatPlygedronInner(float radius, float sideCount );

    static Sd2dRegion flatPlygedronOuter( float radius, float sideCount );

    static Sd2dRegion flatTrapezoid( float lenghtTop, float lenghtBot, float width );

    static Sd2dRegion flatTrapezoidRound( float lenghtTop, float lenghtBot, float width, float radius );

    static Sd2dRegion flatEquidistant( const Sd2dRegion &r, float distance );

           Sd2dRegion flatFromFace( const Sd3drFace &face, const QMatrix4x4 &map );

    //==============================================================================================
    //  Vertex section - functions for model vertex manipulations
    //

    //!
    //! \brief vertex Retrive vertex by its index
    //! \param index  Index of vertex
    //! \return       Vertex
    //!
    QVector3D     vertex( int index ) const { return mVertexList.at(index); }

    //!
    //! \brief point Projection of vertex to XY face
    //! \param index Index of vertex
    //! \return      2d point
    //!
    QPointF       point( int index ) const { return mVertexList.at(index).toPointF(); }

    Sd3drRegion   vertexList( const QList<int> &indexList ) const;

    int           vertexAppend( QVector3D v ) { mVertexList.append(v); return mVertexList.count() - 1; }

    int           vertexCount() const { return mVertexList.count(); }



    //==============================================================================================
    //  Matrix section - functions for matrix calculations
    //

    QMatrix4x4    matrixShift( const Sd3drFace &face, float shift );



    //==============================================================================================
    //  Face section - functions for faces generation and conversion
    //

    Sd3drFace     faceFromFlat(const Sd2dRegion &r, const QMatrix4x4 &map, float offset = 0.0 );

    Sd3drFace     faceFromRegion(const Sd3drRegion &r );

    //!
    //! \brief faceFlat         Builds flat face from list of pairs floats
    //! \param firstVertexIndex First vertex index from which builds face
    //! \param pairList         Pair list as offset from current point begining from firstVertexIndex
    //! \param orientation      0 - xy, 1 - yz, 2 - xz
    //! \return
    //!
    Sd3drFace     faceFlat(int firstVertexIndex, const QList<float> &pairList, int orientation );

    //!
    //! \brief faceFlatMatrix Builds flat face from list of pairs float. First pair is source of path and next pairs are offsets from previous pair
    //! \param pairList       List of pairs float. First pair is source of path and next pairs are offsets from previous pair
    //! \param map            Transfer map
    //! \return               Face
    //!
    Sd3drFace     faceFlatMatrix( const QList<float> &pairList, const QMatrix4x4 &map );

    //!
    //! \brief faceCircle This function builds circle region on base radius with center at 0
    //! \param radius     Radius of builded circle
    //! \param stepDegree Step with which need to create multicorner circle region
    //! \param map        Finish circle transformation map
    //! \return           Circle
    //!
    Sd3drFace     faceCircle( float radius, float stepDegree, const QMatrix4x4 &map );

    //!
    //! \brief faceCircleSide Builds circle region interpolated polygon on base radius with center at 0 and sideCount sides
    //! \param radius         Radius of builded circle
    //! \param sideCount      Side count of polygon
    //! \param map            Finish circle transformation map
    //! \return               Circle
    //!
    Sd3drFace     faceCircleSide( float radius, int sideCount, const QMatrix4x4 &map );

    Sd3drFace     faceEllipse( float radiusx, float radiusy, float stepDegree, const QMatrix4x4 &map );

    Sd3drFace     faceEllipseSide( float radiusx, float radiusy, int sideCount, const QMatrix4x4 &map );

    //!
    //! \brief faceRectangle Builds rectangle region with center at 0 and four edges
    //! \param lenght        Lenght of rectangle (X)
    //! \param width         Width of rectangle (Y)
    //! \param map           Finish rectangle transformation map
    //! \return              Rectangle
    //!
    Sd3drFace     faceRectangle( float lenght, float width, const QMatrix4x4 &map );

    Sd3drFace     faceRectangleRight( float lenght, float width, const QMatrix4x4 &map );

    Sd3drFace     faceRectangleRound( float lenght, float width, float radius, float stepDegree, const QMatrix4x4 &map );

    //!
    //! \brief solidBeveledBox Builds a rectangular box with beveled vertical edges, optionally without bottom
    //! \param lenght          Box length (X-axis)
    //! \param width           Box width (Y-axis)
    //! \param height          Box height (Z-axis)
    //! \param bevelSize       Size of the bevel
    //! \param bevelCount      Number of bevel segments
    //! \param map             Transformation matrix
    //! \param addBot          If true, adds bottom face; if false, bottom face is omitted
    //! \return                List of faces forming the solid
    //!
    Sd3drFaceList faceRectangleBeveled(float lenght, float width, float bevelSize, float bevelCount, const QMatrix4x4 &map );


    //!
    //! \brief faceFromUmPolygon Builds face from region defined in um
    //! \param poly              Source region
    //! \param map               Map to convert result
    //! \return                  Face
    //!
    Sd3drFace     faceFromUmPolygon( const QPolygonF &poly, const QMatrix4x4 &map );

    //!
    //! \brief faceRectangleSide Builds rectangle region with center at 0 and sideCount sides reorganized to rectangle
    //! \param width             Lenght of rectangle (X)
    //! \param lenght            Width of rectangle (Y)
    //! \param sideCount         Side count of polygon
    //! \param map               Finish rectangle transformation map
    //! \return                  Rectangle
    //!
    Sd3drFace     faceRectangleSide( float lenght, float width, int sideCount, const QMatrix4x4 &map );

    Sd3drFace     faceDuplicate( const Sd3drFace &face, const QMatrix4x4 &map );

    Sd3drFace     faceDuplicateShift( const Sd3drFace &face, float shift ) { return faceDuplicate( face, matrixShift( face, shift ) ); }

    Sd3drFace     faceDuplicateScale( const Sd3drFace &face, float scalex, float scaley, float shift );

    Sd3drFace     faceDuplicateGrow( const Sd3drFace &face, float deltax, float deltay, const QMatrix4x4 &map );

    Sd3drFace     faceDuplicateOffset( const Sd3drFace &face, QVector3D offset );

    Sd3drFace     faceTrapezoidZ( const Sd3drFace &face, float height, float lessX, float lessY, float topShiftX, float topShiftY );

    Sd3drFace     faceCurveXZ( const Sd3drFace &face, float radius, float angleSrc, float angleDst );

    //!
    //! \brief faceEqudistante Calculate equidistant face of give face with distance. At last make transformation with map
    //! \param face            Source face
    //! \param distance        Distance of new face
    //! \param map             Finish translation map
    //! \return                Equidistant face
    //!
    Sd3drFace     faceEqudistanteXY( const Sd3drFace &face, float distance, const QMatrix4x4 &map );

    //!
    //! \brief faceBevelXY Calculate finish round face of given face with radius and with shift with given direction
    //! \param face        Source face
    //! \param radius      Distance of new face
    //! \return            Shifted round face
    //!
    Sd3drFace     faceBevelXY(const Sd3drFace &face, float radius);

    static Sd3drFace     facePart( const Sd3drFace &face, const QList<float> &indexes );



    //==============================================================================================
    //  Face list section - functions for face list generation and conversion
    //


    Sd3drFaceList faceListDuplicate(const Sd3drFaceList &faceList, const QMatrix4x4 &map );

    Sd3drFaceList faceListSimplifyXY(const Sd3drFace &face );

    Sd3drFaceList faceListHolesXY(const Sd3drFace &srcFace, const Sd3drFaceList &holeList );

    //!
    //! \brief faceListExtrude Extrudes model from source face in direction specified by map
    //! \param face            Source face of model
    //! \param map             Direction of extruding
    //! \param addBot          Add bottom face to result face list
    //! \param addTop          Add top face to result face list
    //! \return                Solid model: floor, roof and walls
    //!
    Sd3drFaceList faceListExtrude( const Sd3drFace &face, const QMatrix4x4 &map, bool addBot = true, bool addTop = true );

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
    Sd3drFaceList faceListExtrudeShift( const Sd3drFace &face, float shift, bool addBot = true, bool addTop = true);


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
    Sd3drFaceList faceListBodyBeveled(float bodyLenght, float bodyWidth, float bodyHeight, float bevelFront, float bevelSide, float verticalHeight );

    //!
    //! \brief faceListBox  Builds box model from its size
    //! \param lenght       Lenght of box (x)
    //! \param width        Width of box (y)
    //! \param height       Height of box (z)
    //! \param map          Mapping matrix for box
    //! \return             Box model
    //!
    Sd3drFaceList faceListBox(float lenght, float width, float height , const QMatrix4x4 &map);

    //!
    //! \brief faceListCylinder  Builds cylinder model from its size
    //! \param radius            Radius of circle of footing of cylinder
    //! \param height            Height of cylinder
    //! \return                  Cylinder model
    //!
    Sd3drFaceList faceListCylinder( float radius, float height, const QMatrix4x4 &map );

    //!
    //! \brief sd3dModelHexagon Builds hexagonal box body of part
    //! \param lenght           Lenght of box
    //! \param topLenght        Top lenght excluding bevels
    //! \param height           Height of box
    //! \param width            Width of box
    //! \param color            Color of faces of model
    //! \return                 Model of hexagonal box body of part
    //!
    Sd3drFaceList faceListHexagon(float lenght, float topLenght, float height, float width);



    Sd3drFaceList faceListCurveXZ(const Sd3drFace &face, float radius, float angleSrc, float angleDst, int sideCount , bool close);


    Sd3drFaceList faceListPinCurveOne(const Sd3drFace &face, float firstLen, float middleLen, float radius, float angle, int sideCount );

    Sd3drFaceList faceListPinCurveTwo(const Sd3drFace &face, float firstLen, float middleLen, float lastLen, float radius, float angleFirst, float angleSecond, int sideCount );


    Sd3drFaceList faceListPinTqfp(float width, float thickness, float fullLenght, float plateLenght, float height );

    Sd3drFaceList faceListWallRound(const Sd3drFace &face1, const Sd3drFace &face2, float scaleX, float scaleY, float radius, float stepDegree );

    Sd3drFaceList faceListWallBevelXY( const Sd3drFace &face1, const Sd3drFace &face2, float radius, float stepDegree );

    Sd3drFaceList faceListWallDoubleBevelXY(const Sd3drFace &face1, const Sd3drFace &face2, float radius1, float radius2, float stepDegree1, float stepDegree2, float height );

    Sd3drFaceList faceListRotation(const QList<float> &pairs, float angleStart, float angleStop , int sideCount, QMatrix4x4 transfer);

    //!
    //! \brief faceListWalls Builds walls on base layers, count of element each of them is equals each other
    //! \param layers        List of faces throught which will be builds walls
    //! \param close         If true then append wall with n-1 and 0 index vertex on each layer
    //! \return              List of walls
    //!
    static Sd3drFaceList faceListWalls( const Sd3drFaceList &layers, bool close );

    //!
    //! \brief faceListWalls  Builds walls on base bottom and top regions. Walls builded with color
    //! \param face1          First bound of walls - Bottom
    //! \param face2          Second bound of walls - Top
    //! \param close          If true then append wall with n-1 and 0 index vertex
    //! \return               List of walls
    //!
    static Sd3drFaceList faceListWall( const Sd3drFace &face1, const Sd3drFace &face2, bool close );

    static Sd3drFaceList faceListWallList( const Sd3drFaceList &faceList1, const Sd3drFaceList &faceList2, bool close );

    static Sd3drFaceList faceListWallIndexed( const Sd3drFace &face1, const Sd3drFace &face2, const QList<float> &indexes );

    static Sd3drFaceList faceListIndexed( const Sd3drFaceList &faceList, const QList<float> &indexes );

    static Sd3drFaceList ring(const Sd2dRegion &r1, const Sd3drFace &face1, const Sd2dRegion &r2, const Sd3drFace &face2 );

    static QList<float>  afloatArc(float radius, float angleStart, float angleStop, int sideCount);

    //==============================================================================================
    //  Solid figures, modern face list section - functions for face list generation and conversion
    //

    //!
    //! \brief matrixTop    Builds a matrix that translates to the center of the face and orients the z-axis along the normal
    //! \param face         Source face
    //! \param height       Offset face center by normal
    //! \param invertNormal Change normal direction to inverted
    //! \return             Transformation matrix
    //!
    QMatrix4x4    matrixTop(const Sd3drFace &face, float height, bool invertNormal = false );

    //!
    //! \brief matrixTop Builds a matrix that translates to the center of the face and orients the z-axis along the normal
    //! \param faceList  List of faces
    //! \param height    Offset face center by normal
    //! \return          Transformation matrix
    //!
    //!  Uses the last face from the list as the source face
    QMatrix4x4    matrixTop( const Sd3drFaceList &faceList, float height );

    //!
    //! \brief matrixBot Builds a matrix that translates to the center of the face and orients the z-axis opposite to the normal
    //! \param face      Source face
    //! \param height    Offset face center by normal
    //! \return          Transformation matrix
    //!
    QMatrix4x4    matrixBot( const Sd3drFace &face, float height );

    //!
    //! \brief matrixBot Builds a matrix that translates to the center of the face and orients the z-axis opposite to the normal
    //! \param faceList  List of faces
    //! \param height    Offset face center by normal
    //! \return          Transformation matrix
    //!
    //! Uses the last face from the list as the source face
    QMatrix4x4    matrixBot( const Sd3drFaceList &faceList, float height );

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
    QMatrix4x4    matrixNew( float angX, float angY, float angZ, float offX, float offY, float offZ );


    //!
    //! \brief solidBox Builds a rectangular box, optionally without bottom
    //! \param lenght   Box length (X-axis)
    //! \param width    Box width (Y-axis)
    //! \param height   Box height (Z-axis)
    //! \param map      Transformation matrix
    //! \param addBot   If true, adds bottom face; if false, bottom face is omitted
    //! \return         List of faces forming the solid
    //!
    Sd3drFaceList solidBox(float lenght, float width, float height, const QMatrix4x4 &map, bool addBot );

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
    Sd3drFaceList solidBoxWithCone(float lenght, float width, float height, float coneHeight, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidBoxBevel   Builds a rectangular box with beveled vertical edges, optionally without bottom
    //! \param lenght          Box length (X-axis)
    //! \param width           Box width (Y-axis)
    //! \param height          Box height (Z-axis)
    //! \param bevelSize       Size of the bevel
    //! \param bevelCount      Number of bevel segments
    //! \param map             Transformation matrix
    //! \param addBot          If true, adds bottom face; if false, bottom face is omitted
    //! \return                List of faces forming the solid
    //!
    Sd3drFaceList solidBoxBevel(float lenght, float width, float height, float bevelSize, float bevelCount, const QMatrix4x4 &map, bool addBot );

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
    Sd3drFaceList solidBoxRound(float lenght, float width, float height, float roundRadius, float roundCount, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidCylinder Builds a cylinder, optionally without bottom
    //! \param radius        Cylinder radius
    //! \param height        Cylinder height
    //! \param map           Transformation matrix
    //! \param addBot        If true, adds bottom face; if false, bottom face is omitted
    //! \return              List of faces forming the solid
    //!
    Sd3drFaceList solidCylinder(float radius, float height, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidCylinderWithCone Builds a cylinder with a cone on top, optionally without bottom
    //! \param radius                Cylinder radius
    //! \param height                Cylinder height
    //! \param coneHeight            Height of the cone
    //! \param map                   Transformation matrix
    //! \param addBot                If true, adds bottom face; if false, bottom face is omitted
    //! \return                      List of faces forming the solid
    //!
    Sd3drFaceList solidCylinderWithCone(float radius, float height, float coneHeight, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidPlygedronInner Builds a regular polygon prism by inscribed circle radius
    //! \param radius              Radius of the inscribed circle
    //! \param height              Prism height
    //! \param sideCount           Number of sides
    //! \param map                 Transformation matrix
    //! \param addBot              If true, adds bottom face; if false, bottom face is omitted
    //! \return                    List of faces forming the solid
    //!
    Sd3drFaceList solidPlygedronInner( float radius, float height, float sideCount, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidPlygedronOuter Builds a regular polygon prism by circumscribed circle radius
    //! \param radius              Radius of the circumscribed circle
    //! \param height              Prism height
    //! \param sideCount           Number of sides
    //! \param map                 Transformation matrix
    //! \param addBot              If true, adds bottom face; if false, bottom face is omitted
    //! \return                    List of faces forming the solid
    //!
    Sd3drFaceList solidPlygedronOuter( float radius, float height, float sideCount, const QMatrix4x4 &map, bool addBot );

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
    Sd3drFaceList solidTrapezoid(float lenghtTop, float lenghtBot, float width, float height, const QMatrix4x4 &map, bool addBot );

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
    Sd3drFaceList solidTrapezoidRound(float lenghtTop, float lenghtBot, float width, float height, float roundRadius, const QMatrix4x4 &map, bool addBot );


    //!
    //! \brief solidTubeBox Builds a tube with rectangular cross-section
    //! \param lenght       Outer length
    //! \param width        Outer width
    //! \param height       Tube height
    //! \param thickness    Wall thickness
    //! \param map          Transformation matrix
    //! \param addBot       If true, adds bottom face; if false, bottom face is omitted
    //! \return             List of faces forming the solid
    //!
    Sd3drFaceList solidTubeBox(float lenght, float width, float height, float thickness, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidTubeBoxRound Builds a tube with rectangular cross-section and rounded corners
    //! \param lenght            Outer length
    //! \param width             Outer width
    //! \param height            Tube height
    //! \param roundRadius       Radius of the rounded corners
    //! \param roundCount        Number of rounding segments
    //! \param thickness         Wall thickness
    //! \param map               Transformation matrix
    //! \param addBot            If true, adds bottom face; if false, bottom face is omitted
    //! \return                  List of faces forming the solid
    //!
    Sd3drFaceList solidTubeBoxRound(float lenght, float width, float height, float roundRadius, float roundCount, float thickness, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidTubeBoxBevel   Builds a tube with rectangular cross-section and beveled corners
    //! \param lenght              Outer length
    //! \param width               Outer width
    //! \param height              Tube height
    //! \param bevelSize           Size of the bevel
    //! \param bevelCount          Number of bevel segments
    //! \param thickness           Wall thickness
    //! \param map                 Transformation matrix
    //! \param addBot              If true, adds bottom face; if false, bottom face is omitted
    //! \return                    List of faces forming the solid
    //!
    Sd3drFaceList solidTubeBoxBevel(float lenght, float width, float height, float bevelSize, float bevelCount, float thickness, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidTubeCylinder Builds a tube with circular cross-section
    //! \param radius            Outer radius
    //! \param height            Tube height
    //! \param thickness         Wall thickness
    //! \param map               Transformation matrix
    //! \param addBot            If true, adds bottom face; if false, bottom face is omitted
    //! \return                  List of faces forming the solid
    //!
    Sd3drFaceList solidTubeCylinder(float radius, float height, float thickness, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidTubePlygedronInner Builds a tube with regular polygon cross-section by inscribed circle radius
    //! \param radius                  Radius of the inscribed circle
    //! \param height                  Tube height
    //! \param sideCount               Number of sides
    //! \param thickness               Wall thickness
    //! \param map                     Transformation matrix
    //! \param addBot                  If true, adds bottom face; if false, bottom face is omitted
    //! \return                        List of faces forming the solid
    //!
    Sd3drFaceList solidTubePlygedronInner( float radius, float height, float sideCount, float thickness, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidTubePlygedronOuter Builds a tube with regular polygon cross-section by circumscribed circle radius
    //! \param radius                  Radius of the circumscribed circle
    //! \param height                  Tube height
    //! \param sideCount               Number of sides
    //! \param thickness               Wall thickness
    //! \param map                     Transformation matrix
    //! \param addBot                  If true, adds bottom face; if false, bottom face is omitted
    //! \return                        List of faces forming the solid
    //!
    Sd3drFaceList solidTubePlygedronOuter( float radius, float height, float sideCount, float thickness, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidTubeTrapezoid Builds a tube with trapezoidal cross-section
    //! \param lenghtTop          Length of the top edge
    //! \param lenghtBot          Length of the bottom edge
    //! \param width              Width
    //! \param height             Tube height
    //! \param thickness          Wall thickness
    //! \param map                Transformation matrix
    //! \param addBot             If true, adds bottom face; if false, bottom face is omitted
    //! \return                   List of faces forming the solid
    //!
    Sd3drFaceList solidTubeTrapezoid(float lenghtTop, float lenghtBot, float width, float height, float thickness, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidTubeTrapezoidRound Builds a tube with trapezoidal cross-section and rounded corners
    //! \param lenghtTop               Length of the top edge
    //! \param lenghtBot               Length of the bottom edge
    //! \param width                   Width
    //! \param height                  Tube height
    //! \param roundRadius             Radius of the rounded corners
    //! \param thickness               Wall thickness
    //! \param map                     Transformation matrix
    //! \param addBot                  If true, adds bottom face; if false, bottom face is omitted
    //! \return                        List of faces forming the solid
    //!
    Sd3drFaceList solidTubeTrapezoidRound(float lenghtTop, float lenghtBot, float width, float height, float roundRadius, float thickness, const QMatrix4x4 &map, bool addBot );


    //!
    //! \brief solidBlindBox Builds a solid with a blind hole of the same profile on top
    //! \param lenght        Outer length
    //! \param width         Outer width
    //! \param height        Total height
    //! \param thickness     Wall thickness around the hole
    //! \param holeDepth     Depth of the blind hole
    //! \param map           Transformation matrix
    //! \param addBot        If true, adds bottom face; if false, bottom face is omitted
    //! \return              List of faces forming the solid
    //!
    Sd3drFaceList solidBlindBox(float lenght, float width, float height, float thickness, float holeDepth, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidBlindCylinder Builds a solid with a blind cylindrical hole on top
    //! \param radius             Outer radius
    //! \param height             Total height
    //! \param thickness          Wall thickness around the hole
    //! \param holeDepth          Depth of the blind hole
    //! \param map                Transformation matrix
    //! \param addBot             If true, adds bottom face; if false, bottom face is omitted
    //! \return                   List of faces forming the solid
    //!
    Sd3drFaceList solidBlindCylinder(float radius, float height, float thickness, float holeDepth, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidBlindPlygedronInner Builds a solid with a blind polygonal hole (by inscribed circle)
    //! \param radius                   Radius of the inscribed circle
    //! \param height                   Total height
    //! \param sideCount                Number of sides
    //! \param thickness                Wall thickness around the hole
    //! \param holeDepth                Depth of the blind hole
    //! \param map                      Transformation matrix
    //! \param addBot                   If true, adds bottom face; if false, bottom face is omitted
    //! \return                         List of faces forming the solid
    //!
    Sd3drFaceList solidBlindPlygedronInner( float radius, float height, float sideCount, float thickness, float holeDepth, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidBlindPlygedronOuter Builds a solid with a blind polygonal hole (by circumscribed circle)
    //! \param radius                   Radius of the circumscribed circle
    //! \param height                   Total height
    //! \param sideCount                Number of sides
    //! \param thickness                Wall thickness around the hole
    //! \param holeDepth                Depth of the blind hole
    //! \param map                      Transformation matrix
    //! \param addBot                   If true, adds bottom face; if false, bottom face is omitted
    //! \return                         List of faces forming the solid
    //!
    Sd3drFaceList solidBlindPlygedronOuter( float radius, float height, float sideCount, float thickness, float holeDepth, const QMatrix4x4 &map, bool addBot );

    //!
    //! \brief solidBlindTrapezoid Builds a solid with a blind trapezoidal hole on top
    //! \param lenghtTop           Length of the top edge
    //! \param lenghtBot           Length of the bottom edge
    //! \param width               Width
    //! \param height              Total height
    //! \param thickness           Wall thickness around the hole
    //! \param holeDepth           Depth of the blind hole
    //! \param map                 Transformation matrix
    //! \param addBot              If true, adds bottom face; if false, bottom face is omitted
    //! \return                    List of faces forming the solid
    //!
    Sd3drFaceList solidBlindTrapezoid(float lenghtTop, float lenghtBot, float width, float height, float thickness, float holeDepth, const QMatrix4x4 &map, bool addBot );


    //!
    //! \brief solid Extrudes a profile along the given face, adds top face, does not add bottom face
    //! \param face Profile face to extrude
    //! \param height Extrusion height
    //! \return List of faces forming the extruded solid
    //!
    Sd3drFaceList solid( const Sd3drFace &face, float height );

    Sd3drFaceList solid( const Sd3drFace &face, float height, const QMatrix4x4 &m, bool addBot );

    Sd3drFaceList solid( const Sd2dRegion &r, float height, const QMatrix4x4 &m, bool addBot );

    Sd3drFaceList solidTube( const Sd3drFace &faceOut, const Sd3drFace &faceIn, float height, const QMatrix4x4 &m, bool addBot );

    Sd3drFaceList solidTube( const Sd2dRegion &rOut, const Sd2dRegion &rIn, float height, const QMatrix4x4 &m, bool addBot );

    Sd3drFaceList solidBlind( const Sd3drFace &faceOut, const Sd3drFace &faceIn, float heightOut, float heightIn, const QMatrix4x4 &m, bool addBot );

    Sd3drFaceList solidBlind( const Sd2dRegion &rOut, const Sd2dRegion &rIn, float height, float blind, const QMatrix4x4 &m, bool addBot );




    // Adding to existing solids
    // The top face of the solid is used as the base, and it is removed from the list
    // The added solid is merged with the existing one and returned as a new solid

    //!
    //! \brief solidAddCone Adds a cone to the solid
    //! \param faceList     Existing solid face list (top face will be removed)
    //! \param coneHeight   Height of the cone
    //! \return             New solid face list with the cone added
    //!
    Sd3drFaceList solidAddCone( const Sd3drFaceList &faceList, float coneHeight );

    //!
    //! \brief solidAddRoofRound Creates a rounded top face (roof)
    //! \param faceList          Existing solid face list (top face will be removed)
    //! \param roundRadius       Radius of the rounding, >0 - convex, <0 - concave
    //! \param great             true - top size greater than source, false = top size less than source
    //! \return                  New solid face list with rounded roof
    //!
    Sd3drFaceList solidAddRoofRound( const Sd3drFaceList &faceList, float roundRadius, bool great );

    //!
    //! \brief solidAddFloorRound Creates a rounded bottom face (floor)
    //! \param faceList           Existing solid face list (bottom face will be removed)
    //! \param roundRadius        Radius of the rounding
    //! \return                   New solid face list with rounded floor
    //!
    Sd3drFaceList solidAddFloorRound( const Sd3drFaceList &faceList, float roundRadius );

    //!
    //! \brief solidAddRoofBevel Creates a beveled top face (roof)
    //! \param faceList          Existing solid face list (top face will be removed)
    //! \param bevelSize         Size of the bevel
    //! \return                  New solid face list with beveled roof
    //!
    Sd3drFaceList solidAddRoofBevel( const Sd3drFaceList &faceList, float bevelSize );

    //!
    //! \brief solidAddFloorBevel Creates a beveled bottom face (floor)
    //! \param faceList           Existing solid face list (bottom face will be removed)
    //! \param bevelSize          Size of the bevel
    //! \return                   New solid face list with beveled floor
    //!
    Sd3drFaceList solidAddFloorBevel( const Sd3drFaceList &faceList, float bevelSize );

    //!
    //! \brief solidAdd Adds an extrusion with the same profile as the top face
    //! \param faceList Existing solid face list (top face will be removed)
    //! \param height   Extrusion height
    //! \return         New solid face list with the added extrusion
    //!
    Sd3drFaceList solidAdd( const Sd3drFaceList &faceList, float height );

    //!
    //! \brief solidAddSolid Adds an extrusion with r profile at the top face
    //! \param faceList      Sorce face list with top face as base for extrusion
    //! \param r             Extrusion profile
    //! \param height        Extrusion height
    //! \param offset        Offset base extrusion from top face of source
    //! \return              New solid face list with the added extrusion
    //!
    Sd3drFaceList solidAddSolid( const Sd3drFaceList &faceList, const Sd2dRegion &r, float height, float offset );

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
    Sd3drFaceList solidAddCurveVector(const Sd3drFaceList &faceList, float radius, int sideCount, float x, float y, float z );

    //!
    //! \brief solidAddCurveXZ Adds a curved bend of the profile with given radius and angle around the Y axis
    //! \param faceList        Existing solid face list (top face will be removed)
    //! \param radius          Bend radius
    //! \param curveAngle      Bend angle in degrees around axiz Y
    //! \param sideCount       Number of segments for the curve
    //! \return                New solid face list with the curved bend
    //!
    //! The initial direction is considered to be the normal of the original profile
    Sd3drFaceList solidAddCurveXZ(const Sd3drFaceList &faceList, float radius, float curveAngle, int sideCount);

    //!
    //! \brief solidAddHole Adds a hole to the top face
    //! \param faceList Existing solid face list (top face will be removed)
    //! \param face Profile of the hole
    //! \return New solid face list with the hole added
    //!
    Sd3drFaceList solidAddHole( const Sd3drFaceList &faceList, const Sd3drFace &face );


    //!
    //! \brief solidAddBox    Adds a rectangular box on top of the solid
    //! \param lenght         Box length
    //! \param width          Box width
    //! \param height         Box height
    //! \param faceList       Existing solid face list (top face will be removed)
    //! \param transferHeight Offset of the new solid's bottom face relative to the existing solid's top face
    //! \return               New solid face list with the box added
    //!
    Sd3drFaceList solidAddBox(float lenght, float width, float height, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidAddBeveledBox Adds a rectangular box with beveled vertical edges on top of the solid
    //! \param lenght             Box length
    //! \param width              Box width
    //! \param height             Box height
    //! \param bevelSize          Size of the bevel
    //! \param bevelCount         Number of bevel segments
    //! \param faceList           Existing solid face list (top face will be removed)
    //! \param transferHeight     Offset of the new solid's bottom face relative to the existing solid's top face
    //! \return                   New solid face list with the box added
    //!
    Sd3drFaceList solidAddBeveledBox(float lenght, float width, float height, float bevelSize, float bevelCount, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidAddRoundBox Adds a rectangular box with rounded vertical edges on top of the solid
    //! \param lenght           Box length
    //! \param width            Box width
    //! \param height           Box height
    //! \param roundRadius      Radius of the rounded corners
    //! \param roundCount       Number of rounding segments
    //! \param faceList         Existing solid face list (top face will be removed)
    //! \param transferHeight   Offset of the new solid's bottom face relative to the existing solid's top face
    //! \return                 New solid face list with the box added
    //!
    Sd3drFaceList solidAddRoundBox(float lenght, float width, float height, float roundRadius, float roundCount, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidAddCylinder Adds a cylinder on top of the solid
    //! \param radius           Cylinder radius
    //! \param height           Cylinder height
    //! \param faceList         Existing solid face list (top face will be removed)
    //! \param transferHeight   Offset of the new solid's bottom face relative to the existing solid's top face
    //! \return                 New solid face list with the cylinder added
    //!
    Sd3drFaceList solidAddCylinder(float radius, float height, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidAddPlygedronInner Adds a regular polygon prism (by inscribed circle) on top of the solid
    //! \param radius                 Radius of the inscribed circle
    //! \param height                 Prism height
    //! \param sideCount              Number of sides
    //! \param faceList               Existing solid face list (top face will be removed)
    //! \param transferHeight         Offset of the new solid's bottom face relative to the existing solid's top face
    //! \return                       New solid face list with the prism added
    //!
    Sd3drFaceList solidAddPlygedronInner( float radius, float height, float sideCount, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidAddPlygedronOuter Adds a regular polygon prism (by circumscribed circle) on top of the solid
    //! \param radius                 Radius of the circumscribed circle
    //! \param height                 Prism height
    //! \param sideCount              Number of sides
    //! \param faceList               Existing solid face list (top face will be removed)
    //! \param transferHeight         Offset of the new solid's bottom face relative to the existing solid's top face
    //! \return                       New solid face list with the prism added
    //!
    Sd3drFaceList solidAddPlygedronOuter( float radius, float height, float sideCount, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidAddTrapezoid Adds a trapezoid on top of the solid
    //! \param lenghtTop         Length of the top edge
    //! \param lenghtBot         Length of the bottom edge
    //! \param width             Width
    //! \param height            Trapezoid height
    //! \param faceList          Existing solid face list (top face will be removed)
    //! \param transferHeight    Offset of the new solid's bottom face relative to the existing solid's top face
    //! \return                  New solid face list with the trapezoid added
    //!
    Sd3drFaceList solidAddTrapezoid(float lenghtTop, float lenghtBot, float width, float height, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidAddRoundTrapezoid Adds a trapezoid with rounded corners on top of the solid
    //! \param lenghtTop              Length of the top edge
    //! \param lenghtBot              Length of the bottom edge
    //! \param width                  Width
    //! \param height                 Trapezoid height
    //! \param roundRadius            Radius of the rounded corners
    //! \param faceList               Existing solid face list (top face will be removed)
    //! \param transferHeight         Offset of the new solid's bottom face relative to the existing solid's top face
    //! \return                       New solid face list with the trapezoid added
    //!
    Sd3drFaceList solidAddRoundTrapezoid(float lenghtTop, float lenghtBot, float width, float height, float roundRadius, const Sd3drFaceList &faceList, float transferHeight );


    //!
    //! \brief solidHoleBox Adds a rectangular blind hole to the top of the solid
    //! \param lenght Hole length
    //! \param width Hole width
    //! \param height Hole depth
    //! \param faceList Existing solid face list (top face will be removed)
    //! \param transferHeight Offset of the hole's bottom relative to the solid's top face
    //! \return New solid face list with the hole added
    //!
    Sd3drFaceList solidHoleBox(float lenght, float width, float height, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidHoleBeveledBox Adds a rectangular blind hole with beveled corners to the top of the solid
    //! \param lenght Hole length
    //! \param width Hole width
    //! \param height Hole depth
    //! \param bevelSize Size of the bevel
    //! \param bevelCount Number of bevel segments
    //! \param faceList Existing solid face list (top face will be removed)
    //! \param transferHeight Offset of the hole's bottom relative to the solid's top face
    //! \return New solid face list with the hole added
    //!
    Sd3drFaceList solidHoleBeveledBox(float lenght, float width, float height, float bevelSize, float bevelCount, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidHoleRoundBox Adds a rectangular blind hole with rounded corners to the top of the solid
    //! \param lenght Hole length
    //! \param width Hole width
    //! \param height Hole depth
    //! \param roundRadius Radius of the rounded corners
    //! \param roundCount Number of rounding segments
    //! \param faceList Existing solid face list (top face will be removed)
    //! \param transferHeight Offset of the hole's bottom relative to the solid's top face
    //! \return New solid face list with the hole added
    //!
    Sd3drFaceList solidHoleRoundBox(float lenght, float width, float height, float roundRadius, float roundCount, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidHoleCylinder Adds a cylindrical blind hole to the top of the solid
    //! \param radius Hole radius
    //! \param height Hole depth
    //! \param faceList Existing solid face list (top face will be removed)
    //! \param transferHeight Offset of the hole's bottom relative to the solid's top face
    //! \return New solid face list with the hole added
    //!
    Sd3drFaceList solidHoleCylinder(float radius, float height, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidHolePlygedronInner Adds a polygonal blind hole (by inscribed circle) to the top of the solid
    //! \param radius Radius of the inscribed circle
    //! \param height Hole depth
    //! \param sideCount Number of sides
    //! \param faceList Existing solid face list (top face will be removed)
    //! \param transferHeight Offset of the hole's bottom relative to the solid's top face
    //! \return New solid face list with the hole added
    //!
    Sd3drFaceList solidHolePlygedronInner( float radius, float height, float sideCount, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidHolePlygedronOuter Adds a polygonal blind hole (by circumscribed circle) to the top of the solid
    //! \param radius Radius of the circumscribed circle
    //! \param height Hole depth
    //! \param sideCount Number of sides
    //! \param faceList Existing solid face list (top face will be removed)
    //! \param transferHeight Offset of the hole's bottom relative to the solid's top face
    //! \return New solid face list with the hole added
    //!
    Sd3drFaceList solidHolePlygedronOuter( float radius, float height, float sideCount, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidHoleTrapezoid Adds a trapezoidal blind hole to the top of the solid
    //! \param lenghtTop Length of the top edge
    //! \param lenghtBot Length of the bottom edge
    //! \param width Width
    //! \param height Hole depth
    //! \param faceList Existing solid face list (top face will be removed)
    //! \param transferHeight Offset of the hole's bottom relative to the solid's top face
    //! \return New solid face list with the hole added
    //!
    Sd3drFaceList solidHoleTrapezoid(float lenghtTop, float lenghtBot, float width, float height, const Sd3drFaceList &faceList, float transferHeight );

    //!
    //! \brief solidHoleRoundTrapezoid Adds a trapezoidal blind hole with rounded corners to the top of the solid
    //! \param lenghtTop Length of the top edge
    //! \param lenghtBot Length of the bottom edge
    //! \param width Width
    //! \param height Hole depth
    //! \param roundRadius Radius of the rounded corners
    //! \param faceList Existing solid face list (top face will be removed)
    //! \param transferHeight Offset of the hole's bottom relative to the solid's top face
    //! \return New solid face list with the hole added
    //!
    Sd3drFaceList solidHoleRoundTrapezoid(float lenghtTop, float lenghtBot, float width, float height, float roundRadius, const Sd3drFaceList &faceList, float transferHeight );


    //==============================================================================================
    //  Misc section - some service functions
    //

    //!
    //! \brief instanceAppend Appends instance to instance list
    //! \param inst           Instance to append
    //!
    void       instanceAppend( const Sd3drInstance &inst ) { mInstanceList.append(inst); }


    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    void       json( SvJsonWriter3d &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    void       json(const SvJsonReader3d &js );

    //!
    //! \brief draw3d Draws model in 3d space
    //! \param f      OpenGL functions
    //! \param mat    Conversion matrix for model
    //!
    void       draw3d(QOpenGLFunctions_2_0 *f, QMatrix4x4 mat = QMatrix4x4() ) const;

    //!
    //! \brief volumeAdd Append volume of model to result volume
    //! \param volume    Source and result volume
    //!
    void       volumeAdd( QMatrix2x3 &volume ) const;

  private:
    void       faceSizeXY( const Sd3drFace &face, float &sizex, float &sizey ) const;

  };

#endif // SD3DMODEL_H
