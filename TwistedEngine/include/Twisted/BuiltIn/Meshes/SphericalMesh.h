#pragma once

#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Data/MeshData.h"
#include "Twisted/AssetsLayer/AssetsService.h"

#include "Twisted/AssetsLayer/AssetUuid.h"

#include <string>

namespace Twisted::Collections
{
	constexpr AssetUuid sphericalSphereMeshUUID{ 0xD134F531D88B4C4DULL, 0xBAE64CEBBA0D92B2ULL }; // "8b71a6de763d4c4ba3ccd87f464edd20"
	const std::string sphericalSphereMeshName = "sphericalSphere";

    inline PackedMeshData CreateSphericalSpherePackedData()
    {
        MeshData sphereData;

        const float radius = 0.5f;
        const int stacks = 16;   // latitude segments
        const int slices = 32;   // longitude segments

        for (int i = 0; i <= stacks; ++i)
        {
            float v = float(i) / stacks;
            float phi = v * glm::pi<float>(); // 0 → PI

            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            for (int j = 0; j <= slices; ++j)
            {
                float u = float(j) / slices;
                float theta = u * 2.0f * glm::pi<float>(); // 0 → 2PI

                float sinTheta = std::sin(theta);
                float cosTheta = std::cos(theta);

                // Point on sphere surface
                Vec3f normal = {
                    sinPhi * cosTheta,
                    cosPhi,
                    sinPhi * sinTheta
                };

                Vec3f position = normal * radius;

                Vec2f uv = { u, 1.0f - v }; // flip V for typical UV layout

                sphereData.AddPosition(position);
                sphereData.AddNormal(normal);
                sphereData.AddTexCoord(uv);
            }
        }

        // Build index buffer
        for (int i = 0; i < stacks; ++i)
        {
            for (int j = 0; j < slices; ++j)
            {
                int row1 = i * (slices + 1);
                int row2 = (i + 1) * (slices + 1);

                int a = row1 + j;
                int b = row1 + j + 1;
                int c = row2 + j;
                int d = row2 + j + 1;

                // Two triangles per quad, CCW
                sphereData.Indices.push_back(a);
                sphereData.Indices.push_back(c);
                sphereData.Indices.push_back(b);

                sphereData.Indices.push_back(b);
                sphereData.Indices.push_back(c);
                sphereData.Indices.push_back(d);
            }
        }

        return sphereData.Pack();
    }

    void ImportSphericalSphereMesh()
    {
        Mesh* sphereMesh = TObject::Create<Mesh>(Collections::sphericalSphereMeshName);
        sphereMesh->SetData(Collections::CreateSphericalSpherePackedData(), MeshDrawType::STATIC);

        Application::GetInstance().GetService<AssetsService>()->AddBuiltIn(Collections::quadMeshUUID, sphereMesh);
    }
}

//unsigned int addVertex(const Vec3f& p, float radius) {
	//	Vec3f norm = p.normalized();
	//	vertices.push_back(norm * radius);
	//	return static_cast<Index>(vertices.size() - 1);
	//}

	//void generateIcoSphere(float radius, int subdivision)
	//{


	//	/*middlePointIndexCache.clear();


	//	addVertex({ -1,  t,  0 }, radius);
	//	addVertex({ 1,  t,  0 }, radius);
	//	addVertex({ -1, -t,  0 }, radius);
	//	addVertex({ 1, -t,  0 }, radius);

	//	addVertex({ 0, -1,  t }, radius);
	//	addVertex({ 0,  1,  t }, radius);
	//	addVertex({ 0, -1, -t }, radius);
	//	addVertex({ 0,  1, -t }, radius);

	//	addVertex({ t,  0, -1 }, radius);
	//	addVertex({ t,  0,  1 }, radius);
	//	addVertex({ -t,  0, -1 }, radius);
	//	addVertex({ -t,  0,  1 }, radius);

	//	std::vector<Triangle> faces = {
	//		{0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
	//		{1, 5, 9},  {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
	//		{3, 9, 4},  {3, 4, 2},  {3, 2, 6},  {3, 6, 8},  {3, 8, 9},
	//		{4, 9, 5},  {2, 4, 11}, {6, 2, 10}, {8, 6, 7},  {9, 8, 1}
	//	};

	//	for (int i = 0; i < subdivision; ++i) {
	//		std::vector<Triangle> newFaces;
	//		for (const Triangle& tri : faces) {
	//			Index a = getMiddlePoint(tri.v1, tri.v2, radius);
	//			Index b = getMiddlePoint(tri.v2, tri.v3, radius);
	//			Index c = getMiddlePoint(tri.v3, tri.v1, radius);

	//			newFaces.push_back({ tri.v1, a, c });
	//			newFaces.push_back({ tri.v2, b, a });
	//			newFaces.push_back({ tri.v3, c, b });
	//			newFaces.push_back({ a, b, c });
	//		}
	//		faces = std::move(newFaces);
	//	}

	//	for (const Triangle& tri : faces) {
	//		indices.push_back(tri.v1);
	//		indices.push_back(tri.v2);
	//		indices.push_back(tri.v3);
	//	}*/
	//}
