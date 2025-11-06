#pragma once

#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Mesh.h"

#include <string>
#include <vector>

namespace Twisted::Collections
{
	//unsigned int addVertex(const Vec3f& p, float radius) {
	//	Vec3f norm = p.normalized();
	//	vertices.push_back(norm * radius);
	//	return static_cast<Index>(vertices.size() - 1);
	//}

	void generateIcoSphere(float radius, int subdivision)
	{


		/*middlePointIndexCache.clear();


		addVertex({ -1,  t,  0 }, radius);
		addVertex({ 1,  t,  0 }, radius);
		addVertex({ -1, -t,  0 }, radius);
		addVertex({ 1, -t,  0 }, radius);

		addVertex({ 0, -1,  t }, radius);
		addVertex({ 0,  1,  t }, radius);
		addVertex({ 0, -1, -t }, radius);
		addVertex({ 0,  1, -t }, radius);

		addVertex({ t,  0, -1 }, radius);
		addVertex({ t,  0,  1 }, radius);
		addVertex({ -t,  0, -1 }, radius);
		addVertex({ -t,  0,  1 }, radius);

		std::vector<Triangle> faces = {
			{0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
			{1, 5, 9},  {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
			{3, 9, 4},  {3, 4, 2},  {3, 2, 6},  {3, 6, 8},  {3, 8, 9},
			{4, 9, 5},  {2, 4, 11}, {6, 2, 10}, {8, 6, 7},  {9, 8, 1}
		};

		for (int i = 0; i < subdivision; ++i) {
			std::vector<Triangle> newFaces;
			for (const Triangle& tri : faces) {
				Index a = getMiddlePoint(tri.v1, tri.v2, radius);
				Index b = getMiddlePoint(tri.v2, tri.v3, radius);
				Index c = getMiddlePoint(tri.v3, tri.v1, radius);

				newFaces.push_back({ tri.v1, a, c });
				newFaces.push_back({ tri.v2, b, a });
				newFaces.push_back({ tri.v3, c, b });
				newFaces.push_back({ a, b, c });
			}
			faces = std::move(newFaces);
		}

		for (const Triangle& tri : faces) {
			indices.push_back(tri.v1);
			indices.push_back(tri.v2);
			indices.push_back(tri.v3);
		}*/
	}


	constexpr AssetUuid triangleMeshUUID{ 0x9B3D3E56D2414C2CULL, 0xA583EFB9E6C23F71ULL }; // "34ecc86a943e462baa021b6463a3c4b6"
	const std::string triangleMeshName = "triangle";
	const MeshData triangleMesh = {
		{
			Vertex{Vec3f(-0.5f, -0.43301f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f), Vec2f(0.0f, 0.0f)}, // left bottom
			Vertex{Vec3f(0.5f, -0.43301f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f), Vec2f(1.0f, 0.0f)}, // right bottom
			Vertex{Vec3f(0.0f,  0.43301f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f), Vec2f(0.5f, 1.0f)}  // top
		},
		{
			0, 1, 2
		}
	};

	constexpr AssetUuid quadMeshUUID{ 0xE83D8B41FCA64C4CULL, 0xB28CEFA452129D82ULL };  // "45d5c7eb1b11457790d86831a3982298"
	const std::string quadMeshName = "quad";
	const MeshData quadMesh = {
		{
			Vertex{Vec3f(-0.5f, -0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(0.0f,0.0f)}, //a
			Vertex{Vec3f(0.5f, -0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(1.0f,0.0f)}, //b
			Vertex{Vec3f(0.5f, 0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(1.0f,1.0f)}, //c
			Vertex{Vec3f(-0.5f, 0.5f, 0.0f),Vec3f(0.0f,0.0f,1.0f),Vec2f(0.0f,1.0f)}, //d
		},
		{
			0,1,3,
			1,2,3
		}
	};

	constexpr AssetUuid cubeMeshUUID{ 0xAAE4B2C9244E4D36ULL, 0xA7320DBB31C7B18FULL }; // "b1ad370d45c144ab88f20165f9059f7e"
	const std::string cubeMeshName = "cube";
	const MeshData cubeMesh = {
		{
			Vertex{Vec3f(-0.5f, -0.5f, 0.5f),Vec3f(0.0f,0.0f,1.0f),Vec2f(0.0f,0.0f)},//a
			Vertex{Vec3f(0.5f, -0.5f, 0.5f),Vec3f(0.0f,0.0f,1.0f),Vec2f(1.0f,0.0f)},//b
			Vertex{Vec3f(0.5f,  0.5f, 0.5f),Vec3f(0.0f,0.0f,1.0f),Vec2f(1.0f,1.0f)},//c
			Vertex{Vec3f(-0.5f,  0.5f, 0.5f),Vec3f(0.0f,0.0f,1.0f),Vec2f(0.0f,1.0f)},//d

			Vertex{Vec3f(0.5f, -0.5f, 0.5f),Vec3f(1.0f,0.0f,0.0f),Vec2f(0.0f,0.0f)},//b
			Vertex{Vec3f(0.5f, -0.5f,  -0.5f),Vec3f(1.0f,0.0f,0.0f),Vec2f(1.0f,0.0f)},//f
			Vertex{Vec3f(0.5f,  0.5f,  -0.5f),Vec3f(1.0f,0.0f,0.0f),Vec2f(1.0f,1.0f)},//g
			Vertex{Vec3f(0.5f,  0.5f, 0.5f),Vec3f(1.0f,0.0f,0.0f),Vec2f(0.0f,1.0f)},//c

			Vertex{Vec3f(0.5f, -0.5f,  -0.5f),Vec3f(0.0f,0.0f,-1.0f),Vec2f(0.0f,0.0f)},//f
			Vertex{Vec3f(-0.5f, -0.5f,  -0.5f),Vec3f(0.0f,0.0f,-1.0f),Vec2f(1.0f,0.0f)},//e
			Vertex{Vec3f(-0.5f,  0.5f,  -0.5f),Vec3f(0.0f,0.0f,-1.0f),Vec2f(1.0f,1.0f)},//h
			Vertex{Vec3f(0.5f,  0.5f,  -0.5f),Vec3f(0.0f,0.0f,-1.0f),Vec2f(0.0f,1.0f)},//g

			Vertex{Vec3f(-0.5f, -0.5f, -0.5f),Vec3f(-1.0f,0.0f,0.0f),Vec2f(0.0f,0.0f)},//e
			Vertex{Vec3f(-0.5f, -0.5f, 0.5f),Vec3f(-1.0f,0.0f,0.0f),Vec2f(1.0f,0.0f)},//a
			Vertex{Vec3f(-0.5f,  0.5f, 0.5f),Vec3f(-1.0f,0.0f,0.0f),Vec2f(1.0f,1.0f)},//d
			Vertex{Vec3f(-0.5f,  0.5f, -0.5f),Vec3f(-1.0f,0.0f,0.0f),Vec2f(0.0f,1.0f)},//h

			Vertex{Vec3f(0.5f, -0.5f, 0.5f),Vec3f(0.0f,-1.0f,0.0f),Vec2f(0.0f,0.0f)},//b
			Vertex{Vec3f(-0.5f, -0.5f, 0.5f),Vec3f(0.0f,-1.0f,0.0f),Vec2f(1.0f,0.0f)},//a
			Vertex{Vec3f(-0.5f, -0.5f, -0.5f),Vec3f(0.0f,-1.0f,0.0f),Vec2f(1.0f,1.0f)},//e
			Vertex{Vec3f(0.5f, -0.5f, -0.5f),Vec3f(0.0f,-1.0f,0.0f),Vec2f(0.0f,1.0f)},//f

			Vertex{Vec3f(-0.5f,  0.5f, 0.5f),Vec3f(0.0f,1.0f,0.0f),Vec2f(0.0f,0.0f)},//d
			Vertex{Vec3f(0.5f,  0.5f, 0.5f),Vec3f(0.0f,1.0f,0.0f),Vec2f(1.0f,0.0f)},//c
			Vertex{Vec3f(0.5f,  0.5f, -0.5f),Vec3f(0.0f,1.0f,0.0f),Vec2f(1.0f,1.0f)},//g
			Vertex{Vec3f(-0.5f,  0.5f, -0.5f),Vec3f(0.0f,1.0f,0.0f),Vec2f(0.0f,1.0f)},//h
		},
		{
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
		}
	};

	constexpr AssetUuid icoSphereMeshUUID{ 0xD134F531D88B4C4DULL, 0xBAE64CEBBA0D92B2ULL }; // "8b71a6de763d4c4ba3ccd87f464edd20"
	const std::string icoSphereMeshName = "isoSphere";

	/* TO USE UUIDS
	{ 0xFBC1E1A05F224861ULL, 0x83D7EF4512F97A55ULL };
	{ 0xC1E53B529AB045D1ULL, 0xB9E45DE3A13B3E4CULL };
	*/
}