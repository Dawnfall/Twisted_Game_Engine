#pragma once

#include "Utils/GlmUtils.h"
#include "Mesh.h"
#include "Data/MeshData.h"
#include "AssetsService.h"

#include "AssetUuid.h"

#include <string>

namespace Twisted::Collections
{
	constexpr AssetUuid quadMeshUUID{ 0xE83D8B41FCA64C4CULL, 0xB28CEFA452129D82ULL };
	inline constexpr const char* quadMeshName = "quad";

	inline PackedMeshData CreateQuadPackedData()
	{
		MeshData quadData;

		quadData.AddPosition(Vec3f{ -0.5f, 0.0f, -0.5f }); // back-left
		quadData.AddPosition(Vec3f{  0.5f, 0.0f, -0.5f }); // back-right
		quadData.AddPosition(Vec3f{  0.5f, 0.0f,  0.5f }); // front-right
		quadData.AddPosition(Vec3f{ -0.5f, 0.0f,  0.5f }); // front-left

		quadData.AddNormal(Vec3f(0.0f, 1.0f, 0.0f));
		quadData.AddNormal(Vec3f(0.0f, 1.0f, 0.0f));
		quadData.AddNormal(Vec3f(0.0f, 1.0f, 0.0f));
		quadData.AddNormal(Vec3f(0.0f, 1.0f, 0.0f));

		quadData.AddTexCoord(Vec2f{ 0.0f, 0.0f }); // bottom-left
		quadData.AddTexCoord(Vec2f{ 1.0f, 0.0f }); // bottom-right
		quadData.AddTexCoord(Vec2f{ 1.0f, 1.0f }); // top-right
		quadData.AddTexCoord(Vec2f{ 0.0f, 1.0f }); // top-left

		// Indices (two triangles)
		quadData.Indices = { 2, 1, 0, 0, 3, 2 };

		return quadData.Pack();
	}

	inline void ImportQuadMesh()
	{
		Mesh* quadMesh = TObject::Create<Mesh>(Collections::quadMeshName);
		quadMesh->SetData(Collections::CreateQuadPackedData(), MeshDrawType::STATIC);

		Application::GetInstance().GetService<AssetsService>()->AddBuiltIn(Collections::quadMeshUUID, quadMesh);

	}
}
