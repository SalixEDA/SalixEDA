# solidAddConeColor

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`faceList solidAddConeColor( faceList solid, float coneHeight, float offx, float offy, color coneColor )`

## Description
Adds a cone of an arbitrary color to the last face of the shape, with the ability to offset the vertex from the center.

The last face is typically the top one, but not always. For example, in a cup or a glass, it is the bottom of the inner surface.

## Parameters
- solid - original shape
- coneHeight - height of the cone. Positive numbers form a roof-shaped cone, negative numbers form a depression-shaped cone
- offx - X-offset of the cone vertex along the plane of the last face of the original shape
- offy - Y-offset of the cone vertex along the plane of the last face of the original shape
- coneColor - color of the cone

## Usage example

```
body = solidPlygedronInner( 2, 4, 6, true )

body = solidAddConeColor( body, 3, -1,0.5,  selectColor("#008000") )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidAddConeColor.png)

