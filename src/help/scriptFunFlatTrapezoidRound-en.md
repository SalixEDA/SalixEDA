# flatTrapezoidRound

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`flat flatTrapezoidRound( float lenghtTop, float lenghtBot, float width, float radius )`

## Description
Creates a 2D contour (projection) of a trapezoid with rounded corners.

## Parameters
- lenghtTop - length of the top base
- lenghtBot - length of the bottom base
- width - height of the trapezoid
- radius - rounding radius of the corners

## Usage example

```
f = flatTrapezoidRound(  5, 8, 4, 1 )

body = solidNew( f, 0.2, true )
partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunFlatTrapezoidRound.png)
