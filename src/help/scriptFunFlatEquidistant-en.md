# flatEquidistant

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`flat flatEquidistant( flat profile, float distance )`

## Description
Creates a 2D contour equidistant to the original contour. The distance between
the contours is specified by a number. If the number is positive, the equidistant
contour is inside the original one; if the number is negative, it is outside.

## Parameters
- profile - original contour
- distance - distance to the equidistant contour

## Usage example

```
f = flatCircle( 5 )
fe = flatEquidistant( f, 1 )

body = solidTubeDif( f, fe, 0.2, true )
partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunFlatEquidistant.png)
