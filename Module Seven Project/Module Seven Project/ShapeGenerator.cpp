// take from Jamie King's OpenGL Plane Code video
// https://www.youtube.com/watch?v=DJk-aTmrAlQ

#include "ShapeGenerator.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Vertex.h"
#define PI 3.14159265359
using glm::vec3;
using glm::mat4;
using glm::mat3;
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

ShapeData ShapeGenerator::makePlane(uint dimensions)
{
	ShapeData results;
	// plane coordinates
	GLfloat planeVerts[] = {
		12.0f, -0.5f, 12.0f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,//0
		12.0f, -0.5f, -12.0f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,//1
	   -12.0f, -0.5f, -12.0f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//2
	   -12.0f, -0.5f, -12.0f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//3
	   -12.0f, -0.5f, 12.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//4
		12.0f, -0.5f, 12.0f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,//5

	};
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerColor = 3;
	const GLuint floatPerUV = 2;

	results.numVertices = NUM_ARRAY_ELEMENTS(planeVerts);
	results.vertices = new Vertex[results.numVertices];
	memcpy(results.vertices, planeVerts, sizeof(planeVerts));

	unsigned short nIndices[] =
	{
		0, 1, 2,
		3, 4, 5,
	};

	results.numIndices = NUM_ARRAY_ELEMENTS(planeVerts);
	results.indices = new GLushort[results.numIndices];
	memcpy(results.indices, nIndices, sizeof(nIndices));

	return results;
}

ShapeData ShapeGenerator::makeCube()
{
	ShapeData results;
	// Specifies Normalized Device Coordinates for triangle vertices
	GLfloat verts[] = {
		//Positions          //Normals
			// ------------------------------------------------------
			//Back Face          //Negative Z Normal  Texture Coords.
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,//0
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,//1
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,//2
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,//3
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,//4
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,//5

		   //Front Face         //Positive Z Normal
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,//6
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,//7
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,//8
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,//9
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,//10
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,//11

		  //Left Face          //Negative X Normal
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,//12
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,//13
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,//14
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,//15
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,//16
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,//17

		 //Right Face         //Positive X Normal
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,//18
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,//19
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,//20
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,//21
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,//22
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,//23

		 //Bottom Face        //Negative Y Normal
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,//24
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,//25
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,//26
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,//27
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,//28
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,//29

		//Top Face           //Positive Y Normal
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,//30
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,//31
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,//32
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,//33
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,//34
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,//35
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerColor = 3;
	const GLuint floatPerUV = 2;

	results.numVertices = NUM_ARRAY_ELEMENTS(verts);
	results.vertices = new Vertex[results.numVertices];
	memcpy(results.vertices, verts, sizeof(verts));

	unsigned short nIndices[] =
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35
	};
	results.numIndices = NUM_ARRAY_ELEMENTS(verts);
	results.indices = new GLushort[results.numIndices];
	memcpy(results.indices, nIndices, sizeof(nIndices));

	return results;
}

ShapeData ShapeGenerator::makeCylinder()
{
	ShapeData results;
	// Specifies Normalized Device Coordinates for triangle vertices
	GLfloat verts[] = {
		//Positions          //Normals
			// ------------------------------------------------------
			//top circle          //pos y Normal  Texture Coords.
		0.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//0
		1.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//1
		0.867f, 2.0f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//2

		0.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//3
		0.867f,  2.0f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//4
		0.5f, 2.0f, 0.867f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//5

		0.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//6
		0.5f, 2.0f, 0.867f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//7
		0.0f, 2.0f, 1.0f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//8

		0.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//9
		0.0f, 2.0f, 1.0f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//10
	   -0.5f, 2.0f, 0.867f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//11

		0.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//12
	   -0.5f, 2.0f, 0.867f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//13
	   -0.867f, 2.0f, 0.5f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//14

		0.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//15
	   -0.867f, 2.0f, 0.5f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//16
	   -1.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//17

		0.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//18
	   -1.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//19
	   -0.867f, 2.0f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//20

		0.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//21
	   -0.867f, 2.0f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//22
	   -0.5f, 2.0f, -0.867f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//23

		0.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//24
	   -0.5f, 2.0f, -0.867f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//25
		0.0f, 2.0f, -1.0f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//26

		0.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//27
		0.0f, 2.0f, -1.0f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//28
		0.5f, 2.0f, -0.867f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//29

		0.0f,  2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//30
		0.5f, 2.0f, -0.867f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//31
		0.867f, 2.0f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//32

		0.0f,  2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//33
		0.867f, 2.0f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,//34
		1.0f, 2.0f,  0.0f,		0.0f,  1.0f,  0.0f,		0.0f, 1.0f,//35

		// bottom circle    neg y normals
		0.0f,  0.0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//36
		1.0f,  0.0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//37
		0.867f, .0f,  0.5f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//38

		0.0f,  0.0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//39
		0.867f, .0f,  0.5f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//40
		0.5f,  0.0f, 0.867f,	0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//41

		0.0f, 00.0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//42
		0.5f, 0.0f, 0.867f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//43
		0.0f,  00.0f, 1.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//44

		0.0f, 00.0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//45
		0.0f, 000.0f, 1.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//46
	   -0.5f, 0.0f, 0.867f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//47

		0.0f, .000f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//48
	   -0.5f, .00f, 0.867f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//49
	   -0.867f, .00f, 0.5f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//50

		0.0f,  .00f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//51
	   -0.867f, 0.0f, 0.5f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//52
	   -1.0f, 00.0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//53

		0.0f, 00.0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//54
	   -1.0f, .000f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//55
	   -0.867f, .0f, -0.5f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//56

		0.0f,  0.0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//57
	   -0.867f, .0f, -0.5f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//58
	   -0.5f, .0f, -0.867f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//59

		0.0f, 0.00f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//60
	   -0.5f, .0f, -0.867f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//61
		0.0f, .0f, -1.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//62

		0.0f, .0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//63
		0.0f, .0f, -1.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//64
		0.5f, .0f, -0.867f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//65

		0.0f, .0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//66
		0.5f, .0f, -0.867f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//67
		0.867f, .0f, -0.5f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//68

		0.0f,  .0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//69
		0.867f, .0f, -0.5f,		0.0f,  -1.0f,  0.0f,	0.0f, 0.0f,//70
		1.0f, .0f,  0.0f,		0.0f,  -1.0f,  0.0f,	0.0f, 1.0f,//71

		// side
		1.0f, 2.0f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//1
		1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//37
		0.867f, 2.0f, 0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//2

		0.867f, .0f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//38
		0.867f, 2.0f, 0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//2
		1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//37

		0.867f, 2.0f, 0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//4
		0.867f, .0f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//40
		0.5f, 2.0f, 0.867f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//5

		0.5f, 0.0f, 0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//41
		0.5f, 2.0f, 0.867f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//5
		0.867f, .0f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//40

		0.5f, 2.0f, 0.867f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//7
		0.5f, 0.0f, 0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//43
		0.0f, 2.0f, 1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//8

		0.0f, 00.0f, 1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//44
		0.0f, 2.0f, 1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//8
		0.5f, 0.0f, 0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//43

		0.0f, 2.0f, 1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//10
		0.0f, 000.0f, 1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//46
	   -0.5f, 2.0f, 0.867f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//11

	   -0.5f, 0.0f, 0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//47
	   -0.5f, 2.0f, 0.867f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//11
	   0.0f, 000.0f, 1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//46

	   -0.5f, 2.0f, 0.867f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//13
	   -0.5f, .00f, 0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//49
	   -0.867f, 2.0f, 0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//14

	   -0.867f, .00f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//50
	   -0.867f, 2.0f, 0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//14
	   -0.5f, .00f, 0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//49

	   -0.867f, 2.0f, 0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//16
	   -0.867f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//52
	   -1.0f, 2.0f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//17

	   -1.0f, 00.0f, 0.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//53
	   -1.0f, 2.0f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//17
	   -0.867f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//52

	   -1.0f, 2.0f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//19
	   -1.0f, .000f, 0.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//55
	   -0.867f, 2.0f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//20

	   -0.867f, .0f, -0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//56
	   -0.867f, 2.0f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//20
	   -1.0f, .000f, 0.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//55

	   -0.867f, 2.0f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//22
	   -0.867f, .0f, -0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//58
	   -0.5f, 2.0f, -0.867f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//23

	   -0.5f, .0f, -0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//59
	   -0.5f, 2.0f, -0.867f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//23
	   -0.867f, .0f, -0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//58

	   -0.5f, 2.0f, -0.867f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//25
	   -0.5f, .0f, -0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//61
		0.0f, 2.0f, -1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//26

		0.0f, .0f, -1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//62
		0.0f, 2.0f, -1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//26
	   -0.5f, .0f, -0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//61

		0.0f, 2.0f, -1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//28
		0.0f, .0f, -1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//64
		0.5f, 2.0f, -0.867f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//29

		0.5f, .0f, -0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//65
		0.5f, 2.0f, -0.867f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//29
		0.0f, .0f, -1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//64

		0.5f, 2.0f, -0.867f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//31
		0.5f, .0f, -0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//67
		0.867f, 2.0f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//32

		0.867f, .0f, -0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//68
		0.867f, 2.0f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//32
		0.5f, .0f, -0.867f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//67

		0.867f, 2.0f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,//34
		0.867f, .0f, -0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//70
		1.0f, 2.0f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//35

		1.0f, .0f, 0.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,//71
		1.0f, 2.0f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,//35
		0.867f, .0f, -0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,//70
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerColor = 3;
	const GLuint floatPerUV = 2;

	results.numVertices = NUM_ARRAY_ELEMENTS(verts);
	results.vertices = new Vertex[results.numVertices];
	memcpy(results.vertices, verts, sizeof(verts));

	unsigned short nIndices[] =
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9,10,11,
		12,13,14,
		15,16,17,
		18,19,20,
		21,22,23,
		24,25,26,
		27,28,29,
		30,31,32,
		33,34,35,
		36,37,38,
		39,40,41,
		42,43,44,
		45,46,47,
		48,49,50,
		51,52,53,
		54,55,56,
		57,58,59,
		60,61,62,
		63,64,65,
		66,67,68,
		69,70,71,
		1,37,2,
		38,2,37,
		4,40,5,
		41,5,40,
		7,43,8,
		44,8,43,
		10,46,11,
		47,11,46,
		13,49,14,
		50,14,49,
		16,52,17,
		53,17,52,
		19,55,20,
		56,20,55,
		22,58,23,
		59,23,58,
		25,61,26,
		62,26,61,
		28,64,29,
		65,30,64,
		31,67,32,
		68,32,67,
		34,70,35,
		71,35,70,

	};
	results.numIndices = NUM_ARRAY_ELEMENTS(verts);
	results.indices = new GLushort[results.numIndices];
	memcpy(results.indices, nIndices, sizeof(nIndices));

	return results;
}

ShapeData ShapeGenerator::makePyramid()
{
	ShapeData results;
	// Specifies Normalized Device Coordinates for triangle vertices
	GLfloat verts[] = {
		//Positions          //Normals
			// Vertex Positions    // Colors (r,g,b,a)
			 0.5f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 1.0f, //  base red
			 0.5f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
			-0.5f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
			-0.5f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
			-0.5f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,
			 0.5f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 1.0f,

			 0.0f,  1.0f, 0.0f,		1.0f, 0.5f, 0.0f,		.50f, 1.0f,// side yellow
			 0.5f,  0.0f, 0.5f,		1.0f, 0.5f, 0.0f,		1.0f, .0f,
			 0.5f,  0.0f, -0.5f,	1.0f, 0.5f, 0.0f,		.0f, 0.0f,

			 0.0f, 1.0f, 0.0f,		0.0f, -0.5f, 1.0f,		.50f, 1.0f,// side green		 
			 0.5f, 0.0f, -0.5f,		0.0f, -0.5f, 1.0f,		1.0f, 0.0f,
			-0.5f, 0.0f, -0.5f,		0.0f, -0.5f, 1.0f,		0.0f, 0.0f,

			 0.0f, 1.0f, 0.0f,		-1.0f, -0.5f, 0.0f,		.50f, 1.0f,// side blue
			-0.5f, 0.0f, -0.5f,		-1.0f, -0.5f, 0.0f,		1.0f, 0.0f,
			-0.5f, 0.0f, 0.5f,		-1.0f, -0.5f, 0.0f,		0.0f, 0.0f,

			 0.0f, 1.0f, 0.0f,		0.0f, -0.5f, -1.0f,		.50f, 1.0f,// side pink 
			-0.5f, 0.0f, 0.5f,		0.0f, -0.5f, -1.0f,		1.0f, 0.0f,
			 0.5f, 0.0f, 0.5f,		0.0f, -0.5f, -1.0f,		0.0f, 0.0f,
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerColor = 3;
	const GLuint floatPerUV = 2;

	results.numVertices = NUM_ARRAY_ELEMENTS(verts);
	results.vertices = new Vertex[results.numVertices];
	memcpy(results.vertices, verts, sizeof(verts));

	unsigned short nIndices[] =
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
	};
	results.numIndices = NUM_ARRAY_ELEMENTS(verts);
	results.indices = new GLushort[results.numIndices];
	memcpy(results.indices, nIndices, sizeof(nIndices));

	return results;
}
