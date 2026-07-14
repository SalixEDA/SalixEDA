# Script 2.0

In 2026, as part of a major update to the SalixEDA system, an updated set of
scripting functions for creating 3D models was developed and implemented. The
main goal of the update was to simplify the use of scripts for end users.

3D models are described using a scripting language similar to a programming
language. All construction actions are performed by functions, and the objects
themselves can be stored in variables. Below is a typical code for constructing
a through-hole capacitor.

[Full list of functions](script2Root.md)

```
#First place source params of part
bodyDiametr = 6
bodyHeight = 10
bodyColor = selectColor( "#008080" )

#Second place source params of pins
pinDiametr = 0.8
pinLength = 3
pinDistance = 2.5
pinColor = selectColor( "#c0c0c0" )

#========================================================
#Body construction
body = solidCylinder( bodyDiametr / 2, bodyHeight, true )

#Place model into destignation model. After this you can see your construction
partModel = modelSolid( bodyColor, body, 0,0,0,  0,0,0 )

#=========================================================
#Pin construction
pin = solidCylinder( pinDiametr / 2, -pinLength, false )

#First pin placement
pinModel = modelSolid( pinColor, pin, 0,0,0,  -pinDistance / 2,0,0 )

#Second pin placement
pinModel = modelSolidCopy( pinModel, 0,0,0,  pinDistance / 2,0,0 )

#Place pin model into destignation model
partModel = pinModel
```


A three-dimensional model consists of three-dimensional bodies. A model may
contain several copies of the same body, for example, component pins. In any
case, a location and orientation are specified for each body and copy.

A body is constructed from a set of simple shapes (cylinder, parallelepiped, etc.).
These simple shapes are stacked on top of each other. The first simple shape
is defined by one of the [solidXXX](script2Root.md) functions. Subsequent
simple shapes are added to this shape using [solidAddXXX](script2Root.md)
functions. For added simple shapes, the color can also be changed.

A model is formed from a body by specifying its initial color (the color
may change in added shapes), orientation, and position. This is done using
the [modelSolid](scriptFunModelSolid.md) function.

If necessary, a copy of the model can be created. This is done using
the [modelSolidCopy](scriptFunModelSolidCopy.md) function. This function
specifies the orientation and position of the new copy.

All simple bodies are built around the coordinate origin. Additionally, a
body can be constructed using the universal function [solidNew](scriptFunSolidNew.md),
which receives a 2D projection of the body and its height. 2D projections are
also used in the addition functions [solidAddDifXXX](script2Root.md), when the
shape of the added body does not match the shape of the previous one.

The [flatXXX](script2Root.md) functions are used to create 2D shapes (projections).
For example, the function [flatRectangle( float width, float height )](scriptFunFlatRectangle.md) creates a rectangle with the specified dimensions. All shapes are created around the coordinate origin. This means that the geometric center of the shape will be at point 0,0. This can be changed using the [flatOffset( float offx, float offy )](scriptFunFlatOffset.md)
function. This function shifts the geometric center of the shape to the specified point.

The [solidXXX](script2Root.md) functions are used to create 3D shapes.
The arsenal of such shapes includes parallelepipeds, cylinders, tubes, and cups.

The [solidAddXXX](script2Root.md) functions are used to add a 3D protrusion
or recess to the top face. There are function variants that replicate the shape
of the top face of the shape, as well as those that add arbitrary shapes.

The special variable `partModel` is used to register the model as a component
footprint model. Simply assign the finished model to this variable. Assignment
can be done for different parts of the model, for example, in the example above,
the capacitor body model is assigned first, followed by the capacitor pin model.

**Important!** The model is displayed only after assigning it to the partModel
variable. Reading from this variable leads to unpredictable results and is
therefore not allowed.

