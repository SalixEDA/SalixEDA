# solidTrapezoidRound

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`faceList solidTrapezoidRound( float lenghtTop, float lenghtBot, float width, float height, float roundRadius, bool addBottom )`

## Description
Constructs a prism with a rounded-corner trapezoid base.

## Parameters
- lenghtTop - length of the top base of the trapezoid
- lenghtBot - length of the bottom base of the trapezoid
- width - height of the trapezoid (width of the prism)
- height - height of the prism
- roundRadius - rounding radius of the corners
- addBottom - when true, the bottom face is added; otherwise, it is omitted

## Usage example

```
body = solidTrapezoidRound( 2, 4, 2, 3, 0.3, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidTrapezoidRound.png)
