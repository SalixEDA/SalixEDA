# modelSolid

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`model modelSolid( color bodyColor, faceList faces, float angX, float angY, float angZ, float offX, float offY, float offZ )`

## Description
Creates a model based on a body, its color, position, and orientation in space.

## Parameters
- bodyColor - color of the body
- faces - the body obtained by one or more solidXXX functions
- angX, angY, angZ - rotation angles in degrees around the X, Y, and Z axes respectively. The rotation is performed sequentially: first around X, then around Y, and finally around Z
- offX, offY, offZ - spatial offset of the body

## Usage example

```
body = solidBox( 6, 4, 3, true )

partModel = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )
```

This code generates the following image:

![pic](scriptFunSolidBox.png)
