# flatCircle

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`flat flatCircle( float radius )`

## Description
Creates a 2D contour (projection) of a circle. Similar to other contour
generation functions based on curved lines, the circle is formed as a polygon
with 30 segments. If a different number of segments is required, you can use
the [ellipse](scriptFunFlatEllipse.md) function by setting equal radii for both axes.

## Parameters
- radius - radius of the circle

## Usage example

```
f = flatCircle( 5 )

body = solidNew( f, 0.2, true )
partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunFlatCircle.png)
