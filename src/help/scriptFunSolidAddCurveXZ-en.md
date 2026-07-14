# solidAddCurveXZ

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`faceList solidAddCurveXZ( faceList solid, float radius, int sideCount, float curveAngle )`

## Description
Extends the body from the last face by performing a turn in the XZ plane (around the Y axis).

The last face is typically the top one, but not always. For example, in a cup or a glass, it is the bottom of the inner surface.

## Parameters
- solid - original shape
- radius - bend radius
- sideCount - the bend is interpolated by a polyline; this parameter specifies how many segments the bend should be divided into
- curveAngle - rotation angle in degrees. Positive numbers indicate a clockwise rotation, negative numbers indicate a counterclockwise rotation

## Usage example

```
body = solidCylinder( 1, 5, true )

body = solidAddCurveXZ( body, 3, 12, 150 )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidAddCurveXZ.png)

