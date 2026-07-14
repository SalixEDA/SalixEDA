# solidAddCone

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`faceList solidAddCone( faceList solid, float coneHeight )`

## Description
Adds a cone to the last face of the shape.

The last face is typically the top one, but not always. For example, in a glass or a cup, it is the bottom of the inner surface.

## Parameters
- solid - original shape
- coneHeight - height of the cone. Positive numbers form a roof-shaped cone, negative numbers form a depression-shaped cone

## Usage example

```
body = solidPlygedronInner( 2, 4, 6, true )

body = solidAddCone( body, 3 )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidAddCone.png)

