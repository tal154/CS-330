#pragma once
// take from Jamie King's OpenGL Plane Code video
// https://www.youtube.com/watch?v=DJk-aTmrAlQ

#pragma once
#include "ShapeData.h"
typedef unsigned int uint;

class ShapeGenerator
{
public:
	static ShapeData makePlane(uint dimensions = 10);
	static ShapeData makeCube();
	static ShapeData makeCylinder();
	static ShapeData makePyramid();
};
