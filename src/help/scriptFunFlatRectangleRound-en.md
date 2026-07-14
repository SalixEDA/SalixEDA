# flatRectangleRound

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`flat flatRectangleRound( float lenght, float width, float radius, float stepDegree, float count )`

## Description
Creates a 2D contour (projection) of a rectangle with rounded corners.
The rounding radius and the number of rounded corners are configurable.

## Parameters
- lenght - length of the rectangle (X)
- width - width of the rectangle (Y)
- radius - rounding radius
- stepDegree - roundings are formed by a polyline; this parameter sets the step in degrees between the vertices of this polyline
- count - number of rounded corners, from 1 to 4

## Usage example

```
f = flatRectangleRound( 10, 5, 1, 15, 2 )

body = solidNew( f, 0.2, true )
partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunFlatRectangleRound.png)
