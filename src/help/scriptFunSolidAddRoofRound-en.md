# solidAddRoofRound

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`faceList solidAddRoofRound( faceList solid, float roundRadius, bool great )`

## Description
Adds a roof with rounded faces to the last face of the shape. There are 4 possible roof combinations depending on the sign of the radius and the value of the `great` flag.

The last face is typically the top one, but not always. For example, in a cup or a glass, it is the bottom of the inner surface.

## Parameters
- solid - original shape
- roundRadius - rounding radius. If positive, a convex rounding is formed; if negative, a concave rounding is formed
- great - if true, the roof surface is larger than the last face of the shape; if false, it is smaller

## Usage example

```
body = solidPlygedronInner( 2, 4, 6, true )

body = solidAddRoofRound( body, -1, false )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidAddRoofRound.png)

