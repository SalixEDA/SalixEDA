# flatCircularSegment

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`flat flatCircularSegment( float radius, float angleStart, float angleStop )`

## Description
Creates a 2D contour (projection) of a circular segment. The circle unfolds clockwise from the start angle to the end angle.

## Parameters
- radius - radius of the circle
- angleStart - start angle of the segment
- angleStop - end angle of the segment

## Usage example

```
f = flatCircularSegment(  5, 10, 130 )

body = solidNew( f, 0.2, true )
partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunFlatCircularSegment.png)
