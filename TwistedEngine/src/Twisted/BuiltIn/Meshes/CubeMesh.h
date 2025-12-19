#pragma once

#include "Twisted/BuiltIn/BuiltInRegistry.h"

#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Data/MeshData.h"

#include "Twisted/AssetsLayer/AssetUuid.h"
#include "Twisted/AssetsLayer/AssetsService.h"

namespace Twisted::Collections
{
	constexpr AssetUuid cubeMeshUUID{ 0xAAE4B2C9244E4D36ULL, 0xA7320DBB31C7B18FULL };
	const std::string cubeMeshName = "cube";
	inline PackedMeshData CreateCubePackedData()
	{
		MeshData cubeData;

		// Define cube corners
		Vec3f positions[8] = {
			{-0.5f, -0.5f, -0.5f}, // 0
			{ 0.5f, -0.5f, -0.5f}, // 1
			{ 0.5f,  0.5f, -0.5f}, // 2
			{-0.5f,  0.5f, -0.5f}, // 3
			{-0.5f, -0.5f,  0.5f}, // 4
			{ 0.5f, -0.5f,  0.5f}, // 5
			{ 0.5f,  0.5f,  0.5f}, // 6
			{-0.5f,  0.5f,  0.5f}  // 7
		};

		// Face definitions (indices into positions array)
		struct Face { int v0, v1, v2, v3; Vec3f normal; };
		Face faces[6] = {
			{0, 1, 2, 3, { 0,  0, -1}}, // back
			{5, 4, 7, 6, { 0,  0,  1}}, // front
			{4, 0, 3, 7, {-1,  0,  0}}, // left
			{1, 5, 6, 2, { 1,  0,  0}}, // right
			{3, 2, 6, 7, { 0,  1,  0}}, // top
			{4, 5, 1, 0, { 0, -1,  0}}  // bottom
		};

		// Standard UVs for each face
		Vec2f uvs[4] = {
			{0.0f, 0.0f}, // bottom-left
			{1.0f, 0.0f}, // bottom-right
			{1.0f, 1.0f}, // top-right
			{0.0f, 1.0f}  // top-left
		};

		for (auto& face : faces)
		{
			int startIndex = cubeData.Positions.size();

			// Add positions, normals, UVs for this face
			cubeData.AddPosition(positions[face.v0]);  
			cubeData.AddPosition(positions[face.v1]);  
			cubeData.AddPosition(positions[face.v2]);  
			cubeData.AddPosition(positions[face.v3]);  

			cubeData.AddNormal(face.normal);
			cubeData.AddNormal(face.normal);
			cubeData.AddNormal(face.normal);
			cubeData.AddNormal(face.normal);

			cubeData.AddTexCoord(uvs[0]);
			cubeData.AddTexCoord(uvs[1]);
			cubeData.AddTexCoord(uvs[2]);
			cubeData.AddTexCoord(uvs[3]);

			// Two triangles per face
			cubeData.Indices.push_back(startIndex + 2);
			cubeData.Indices.push_back(startIndex + 1);
			cubeData.Indices.push_back(startIndex + 0);

			cubeData.Indices.push_back(startIndex + 0);
			cubeData.Indices.push_back(startIndex + 3);
			cubeData.Indices.push_back(startIndex + 2);
		}

		return cubeData.Pack();
	}

	void ImportCubeMesh()
	{
		Mesh* cubeMesh = TObject::Create<Mesh>(Collections::cubeMeshName);
		Mesh_GL::SetData(*cubeMesh, Collections::CreateCubePackedData(), MeshDrawType::STATIC);
		AssetsService::GetInstance()->AddBuiltIn(Collections::cubeMeshUUID, cubeMesh);
	
	}
}