# solidCylinder

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`faceList solidCylinder( float radius, float height, bool addBottom )`

## Description
Constructs a cylinder.

## Parameters
- radius - base radius
- height - height of the cylinder
- addBottom - when true, the bottom face is added; otherwise, it is omitted

## Usage example

```
body = solidCylinder( 2, 5, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidCylinder.png)
