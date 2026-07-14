# solidAddCurveVector

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`faceList solidAddCurveVector( faceList solid, float radius, int sideCount, float x, float y, float z )`

## Description
Extends the body from the last face by performing a turn in the direction of the specified vector.

The last face is typically the top one, but not always. For example, in a cup or a glass, it is the bottom of the inner surface.

## Parameters
- solid - original shape
- radius - bend radius
- sideCount - the bend is interpolated by a polyline; this parameter specifies how many segments the bend should be divided into
- x,y,z - direction of the bend vector

## Usage example

```
body = solidCylinder( 1, 5, true )

body = solidAddCurveVector( body, 3, 12, 1, 1, 0 )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidAddCurveVector.png)

