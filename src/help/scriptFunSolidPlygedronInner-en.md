# solidPlygedronInner

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`faceList solidPlygedronInner( float radius, float height, float sideCount, bool addBottom )`

## Description
Constructs a prism with a regular polygon base specified by an inscribed circle.

## Parameters
- radius - radius of the circle inscribed into the prism's base polygon
- height - height of the prism
- sideCount - number of sides of the base polygon
- addBottom - when true, the bottom face is added; otherwise, it is omitted

## Usage example

```
body = solidPlygedronInner( 2, 4, 6, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidPlygedronInner.png)
