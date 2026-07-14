# solidBoxRound

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`faceList solidBoxRound( float lenght, float width, float height, float roundRadius, float roundCount, bool addBottom )`

## Description
Constructs a parallelepiped (box). Roundings are added to the vertical edges.

## Parameters
- lenght - base length (X)
- width - base width (Y)
- height - height
- roundRadius - rounding radius
- roundCount - number of rounded edges (1-4)
- addBottom - when true, the bottom face is added; otherwise, it is omitted

## Usage example

```
body = solidBoxRound( 6, 4, 3, 0.5, 3, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidBoxRound.png)
