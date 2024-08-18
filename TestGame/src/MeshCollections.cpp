#include "MeshCollections.h"

#include "Collections/Vector.h"
#include "Collections/Geometry.h"

using Dawn::Vec3f;
using Dawn::Vec2f;
using Dawn::Vertex;

namespace Collections
{
	//*********************
	// triangle
	const std::vector<Vertex> triangleVertices = {
		{Vec3f(-0.5f,-0.43301f,0.0f),Vec3f(),Vec2f()},
		{Vec3f(0.5f,-0.43301f,0.0f),Vec3f(),Vec2f()},
		{Vec3f(0.0f,0.43301f,0.0f),Vec3f(),Vec2f()}
	};

	const std::vector<unsigned int> triangleIndices{
		0, 1, 2
	};

	const std::vector<Vertex> quadVertices{
		{Vec3f(-0.5f, -0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(0.0f,0.0f)}, //a
		{Vec3f(0.5f, -0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(1.0f,0.0f)}, //b
		{Vec3f(0.5f, 0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(1.0f,1.0f)}, //c
		{Vec3f(-0.5f, 0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(0.0f,1.0f)}, //d
	};

	const std::vector<unsigned int> quadIndices{
		0,1,3,1,2,3
	};

	//******************
	// Cube
	const std::vector<Vertex> cubeVertices{
		{Vec3f(-0.5f, -0.5f, 0.5f),Vec3f(0.0f,0.0f,1.0f),Vec2f(0.0f,0.0f)},//a
		{Vec3f(0.5f, -0.5f, 0.5f),	Vec3f(0.0f,0.0f,1.0f),Vec2f(1.0f,0.0f)},//b
		{Vec3f(0.5f,  0.5f, 0.5f),	Vec3f(0.0f,0.0f,1.0f),Vec2f(1.0f,1.0f)},//c
		{Vec3f(-0.5f,  0.5f, 0.5f),Vec3f(0.0f,0.0f,1.0f),Vec2f(0.0f,1.0f)},//d

		{Vec3f(0.5f, -0.5f, 0.5f),	Vec3f(1.0f,0.0f,0.0f),Vec2f(0.0f,0.0f)},//b
		{Vec3f(0.5f, -0.5f,  -0.5f),	Vec3f(1.0f,0.0f,0.0f),Vec2f(1.0f,0.0f)},//f
		{Vec3f(0.5f,  0.5f,  -0.5f),	Vec3f(1.0f,0.0f,0.0f),Vec2f(1.0f,1.0f)},//g
		{Vec3f(0.5f,  0.5f, 0.5f),	Vec3f(1.0f,0.0f,0.0f),Vec2f(0.0f,1.0f)},//c

		{Vec3f(0.5f, -0.5f,  -0.5f),	Vec3f(0.0f,0.0f,-1.0f),Vec2f(0.0f,0.0f)},//f
		{Vec3f(-0.5f, -0.5f,  -0.5f),Vec3f(0.0f,0.0f,-1.0f),Vec2f(1.0f,0.0f)},//e
		{Vec3f(-0.5f,  0.5f,  -0.5f),Vec3f(0.0f,0.0f,-1.0f),Vec2f(1.0f,1.0f)},//h
		{Vec3f(0.5f,  0.5f,  -0.5f),	Vec3f(0.0f,0.0f,-1.0f),Vec2f(0.0f,1.0f)},//g

		{Vec3f(-0.5f, -0.5f, -0.5f),Vec3f(-1.0f,0.0f,0.0f),Vec2f(0.0f,0.0f)},//e
		{Vec3f(-0.5f, -0.5f, 0.5f),Vec3f(-1.0f,0.0f,0.0f),Vec2f(1.0f,0.0f)},//a
		{Vec3f(-0.5f,  0.5f, 0.5f),Vec3f(-1.0f,0.0f,0.0f),Vec2f(1.0f,1.0f)},//d
		{Vec3f(-0.5f,  0.5f, -0.5f),Vec3f(-1.0f,0.0f,0.0f),Vec2f(0.0f,1.0f)},//h

		{Vec3f(0.5f, -0.5f, 0.5f),	Vec3f(0.0f,-1.0f,0.0f),Vec2f(0.0f,0.0f)},//b
		{Vec3f(-0.5f, -0.5f, 0.5f),Vec3f(0.0f,-1.0f,0.0f),Vec2f(1.0f,0.0f)},//a
		{Vec3f(-0.5f, -0.5f, -0.5f),Vec3f(0.0f,-1.0f,0.0f),Vec2f(1.0f,1.0f)},//e
		{Vec3f(0.5f, -0.5f, -0.5f),	Vec3f(0.0f,-1.0f,0.0f),Vec2f(0.0f,1.0f)},//f

		{Vec3f(-0.5f,  0.5f, 0.5f),Vec3f(0.0f,1.0f,0.0f),Vec2f(0.0f,0.0f)},//d
		{Vec3f(0.5f,  0.5f, 0.5f),	Vec3f(0.0f,1.0f,0.0f),Vec2f(1.0f,0.0f)},//c
		{Vec3f(0.5f,  0.5f, -0.5f),	Vec3f(0.0f,1.0f,0.0f),Vec2f(1.0f,1.0f)},//g
		{Vec3f(-0.5f,  0.5f, -0.5f),Vec3f(0.0f,1.0f,0.0f),Vec2f(0.0f,1.0f)},//h
	};

	const std::vector<unsigned int> cubeIndices{
		0,1,2,
		0,2,3,
		4,5,6,
		4,6,7,
		8,9,10,
		8,10,11,
		12,13,14,
		12,14,15,
		16,17,18,
		16,18,19,
		20,21,22,
		20,22,23

	};
}