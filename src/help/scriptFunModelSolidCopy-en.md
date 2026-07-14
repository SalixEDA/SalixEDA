# modelSolidCopy

[Contents](contents.htm)=>[3D Script](script3d.md)=>[Building 3D models](script2Root.md)

---

## Call format
`model modelSolidCopy( model src, float angX, float angY, float angZ, float offX, float offY, float offZ )`

## Description
Creates a copy of the model with a new orientation and position.

Model copying is the core principle used to generate multiple identical component pins or leads.

## Parameters
- src - original model
- angX, angY, angZ - rotation angles in degrees around the X, Y, and Z axes respectively. The rotation is performed sequentially: first around X, then around Y, and finally around Z
- offX, offY, offZ - spatial offset of the body

## Usage example

```
body = solidBox( 6, 4, 3, true )

md = modelSolid( selectColor("#800000"), body, 0,0,0, 0,0,0 )

partModel = modelSolidCopy( md, 0,0,45,   7, 2, 0 )
```

This code generates the following image:

![pic](scriptFunModelSolidCopy.png)
