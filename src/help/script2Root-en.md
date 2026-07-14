# Building 3D Models

[Contents](contents.htm)=>[3D Script](script3d.md)=>Building 3D Models

3D models are described using a scripting language similar to a programming language.
All construction actions are performed by functions, and the objects themselves can be stored
in variables.

Below are lists of functions of the modern scripting language implementation with their brief descriptions.

Functions for creating 2D contours (projections):

| Function Name | Description |
|--------------------------------|--------------|
| [flatRectangle](scriptFunFlatRectangle.md)|Creates a rectangle|
| [flatRectangleBevel](scriptFunFlatRectangleBevel.md) | Creates a rectangle with beveled corners|
| [flatRectangleRound](scriptFunFlatRectangleRound.md) | Creates a rectangle with rounded corners|
| [flatEllipse](scriptFunFlatEllipse.md) |Creates an ellipse|
| [flatCircle](scriptFunFlatCircle.md) |Creates a circle|
| [flatPlygedronInner](scriptFunFlatPlygedron.md)|Creates a regular polygon based on the diameter of the incircle|
| [flatPlygedronOuter](scriptFunFlatPlygedron.md)|Creates a regular polygon based on the diameter of the circumcircle|
| [flatTrapezoid](scriptFunFlatTrapezoid.md)|Creates a trapezoid|
| [flatTrapezoidRound](scriptFunFlatTrapezoidRound.md)|Creates a trapezoid with rounded corners|
| [flatCircularSegment](scriptFunFlatCurcularSegment.md)|Creates a circular segment|
| [flatEquidistant](scriptFunFlatEquidistant.md)|Creates an equidistant region from a given one|
| [flatOffset](scriptFunFlatOffset.md)|Creates an offset region from a given one|

Functions for creating initial solid bodies of simple shapes. These functions work by extruding
volume from a 2D contour (projection):

| Function Name | Description |
|--------------------------------|--------------|
| [solidBox](scriptFunSolidBox.md) |Body with rectangular projection (parallelepiped)|
| [solidBoxWithCone](scriptFunSolidBoxWithCone.md)|Body with rectangular projection with a cone on top|
| [solidBoxBevel](scriptFunSolidBoxBevel.md) |Body with rectangular projection with beveled corners|
| [solidBoxRound](scriptFunSolidBoxRound.md) |Body with rectangular projection with rounded corners|
| [solidCylinder](scriptFunSolidCylinder.md) |Body with circular projection (cylinder)|
| [solidCylinderWithCone](scriptFunSolidCylinderWithCone.md)|Body with circular projection (cylinder) with a cone on top|
| [solidPlygedronInner](scriptFunSolidPlygedronInner.md) |Body with projection of a regular polygon defined by the incircle diameter|
| [solidPlygedronOuter](scriptFunSolidPlygedronOuter.md) |Body with projection of a regular polygon defined by the circumcircle diameter|
| [solidTrapezoid](scriptFunSolidTrapezoid.md) |Body with trapezoidal projection|
| [solidTrapezoidRound](scriptFunSolidTrapezoidRound.md) |Body with projection of a trapezoid with rounded corners|
| [solidRectPrizm](scriptFunSolidRectPrizm.md) |Rectangular prism with two or more levels|

Functions for creating initial solid bodies that work by extruding volume from a given 2D contour (projection):

| Function Name | Description |
|--------------------------------|--------------|
| [solidNew](scriptFunSolidNew.md) |Creates a body from an arbitrary contour|
| [solidTube](scriptFunSolidTube.md) |Creates a tube|
| [solidTubeColor](scriptFunSolidTubeColor.md)|Creates a tube with a specified color for the inner part|
| [solidTubeDif](scriptFunSolidTubeDif.md) |Creates a tube with different shapes for the outer and inner parts|
| [solidTubeDifColor](scriptFunSolidTubeDifColor.md) |Creates a tube with different shapes for the outer and inner parts and specified color for the inner part|

Functions for adding bodies to existing bodies. These functions extract the last face from the existing body (this is usually the top face, but not always. For example, for a cup, this would be the bottom of the inner part)

| Function Name | Description |
|--------------------------------|--------------|
| [solidAddCone](scriptFunSolidAddCone.md) |Adds a cone|
| [solidAddConeColor](scriptFunSolidAddConeColor.md) |Adds a cone with specified color|
| [solidAddRoofRound](scriptFunSolidAddRoofRound.md) |Adds a rounded roof|
| [solidAddRoofBevel](scriptFunSolidAddRoofBevel.md) |Adds a roof with bevels|
| [solidAdd](scriptFunSolidAdd.md) |Adds the same shape as the previous object, but with a size change|
| [solidAddColor](scriptFunSolidAddColor.md) |Adds the same shape as the previous object, but with a size change and specified color for the added shape|
| [solidAddDif](scriptFunSolidAddDif.md) |Adds an arbitrary shape|
| [solidAddDifColor](scriptFunSolidAddDifColor.md) |Adds an arbitrary shape with specified color for the new shape|
| [solidAddCurveVector](scriptFunSolidAddCurveVector.md) |Adds a body bend along a vector|
| [solidAddCurveXZ](scriptFunSolidAddCurveXZ.md) |Adds a body bend at an angle in the XZ plane|
| [solidAddCurveYZ](scriptFunSolidAddCurveYZ.md) |Adds a body bend at an angle in the YZ plane|
| [solidAddHole](scriptFunSolidAddHole.md) |Adds a hole without a bottom|
| [solidAddHoleColor](scriptFunSolidAddHoleColor.md) |Adds a hole without a bottom with specified hole color|
| [solidAddHoleDif](scriptFunSolidAddHoleDif.md) |Adds a hole without a bottom with a different shape than the body|
| [solidAddHoleDifColor](scriptFunSolidAddHoleDifColor.md) |Adds a hole without a bottom with a different shape than the body and specified hole color|
| [solidAddBlindArray](scriptFunSolidAddBlindArray.md) |Adds an array of blind holes (with bottom)|
| [solidAddBlindArrayColor](scriptFunSolidAddBlindArrayColor.md) |Adds an array of blind holes (with bottom) with specified hole color|

Functions for creating models from bodies. A model is one or more bodies for which color and position are specified. You can specify one or several different positions of bodies. This is convenient for pins and other repeating structures.

| Function Name | Description |
|--------------------------------|--------------|
| [modelSolid](scriptFunModelSolid.md) |Creates a model by specifying body color and position|
| [modelSolidAdd](scriptFunModelSolidAdd.md) |Adds a body to the model|
| [modelSolidCopy](scriptFunModelSolidCopy.md) |Adds a new position for the model bodies (copy)|

