# solidBoxBevel

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`faceList solidBoxBevel( float lenght, float width, float height, float bevelSize, float bevelCount, bool addBottom )`

## Description
Constructs a parallelepiped (box). A bevel (chamfer) is added to the vertical edges.

## Parameters
- lenght - base length (X)
- width - base width (Y)
- height - height
- bevelSize - size of the bevel
- bevelCount - number of beveled edges (1-4)
- addBottom - when true, the bottom face is added; otherwise, it is omitted

## Usage example

```
body = solidBoxBevel( 6, 4, 3, 0.5, 2, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidBoxBevel.png)
