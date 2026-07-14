# solidBox

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`faceList solidBox( float lenght, float width, float height, bool addBottom )`

## Description
Constructs a parallelepiped (box).

## Parameters
- lenght - base length (X)
- width - base width (Y)
- height - height
- addBottom - when true, the bottom face is added; otherwise, it is omitted

## Usage example

```
body = solidBox( 6, 4, 3, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidBox.png)
