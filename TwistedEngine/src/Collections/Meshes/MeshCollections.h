#pragma once

#include "twistedpch.h"
#include "Collections/Geometry.h"

using Twisted::Vec3f;
using Twisted::Vec2f;
using Twisted::Vertex;

namespace Twisted::Collections
{
	const std::string triangleMeshName = "triangle";
	const std::vector<Vertex> triangleVertices = {
		{Vec3f(-0.5f,-0.43301f,0.0f),Vec3f(),Vec2f()},
		{Vec3f(0.5f,-0.43301f,0.0f),Vec3f(),Vec2f()},
		{Vec3f(0.0f,0.43301f,0.0f),Vec3f(),Vec2f()}
	};
	const std::vector<unsigned int> triangleIndices = {
		0, 1, 2
	};

	const std::string quadMeshName = "quad";
	const std::vector<Vertex> quadVertices = {
		{Vec3f(-0.5f, -0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(0.0f,0.0f)}, //a
		{Vec3f(0.5f, -0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(1.0f,0.0f)}, //b
		{Vec3f(0.5f, 0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(1.0f,1.0f)}, //c
		{Vec3f(-0.5f, 0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(0.0f,1.0f)}, //d
	};
	const std::vector<unsigned int> quadIndices = {
	0,1,3,1,2,3
	};

	const std::string cubeMeshName = "cube";
	const std::vector<Vertex> cubeVertices = {
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
	const std::vector<unsigned int> cubeIndices = {
		0,2,1,
		0,3,2,
		4,6,5,
		4,7,6,
		8,10,9,
		8,11,10,
		12,14,13,
		12,15,14,
		16,18,17,
		16,19,18,
		20,22,21,
		20,23,22
	};
}