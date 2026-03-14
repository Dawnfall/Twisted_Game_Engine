#pragma once

#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Data/MeshData.h"
#include "Twisted/AssetsLayer/AssetsService.h"

#include "Twisted/AssetsLayer/AssetUuid.h"

#include <string>

namespace Twisted::Collections
{
	constexpr AssetUuid quadMeshUUID{ 0xE83D8B41FCA64C4CULL, 0xB28CEFA452129D82ULL };
	const std::string quadMeshName = "quad";

	inline PackedMeshData CreateQuadPackedData()
	{
		MeshData quadData;

		quadData.AddPosition(Vec3f{ -0.5f, -0.5f, 0.0f }); // bottom-left
		quadData.AddPosition(Vec3f{ 0.5f, -0.5f, 0.0f }); // bottom-right
		quadData.AddPosition(Vec3f{ 0.5f,  0.5f, 0.0f }); // top-right
		quadData.AddPosition(Vec3f{ -0.5f,  0.5f, 0.0f }); // top-left

		quadData.AddNormal(Vec3f(0.0f, 0.0f, -1.0f));
		quadData.AddNormal(Vec3f(0.0f, 0.0f, -1.0f));
		quadData.AddNormal(Vec3f(0.0f, 0.0f, -1.0f));
		quadData.AddNormal(Vec3f(0.0f, 0.0f, -1.0f));

		quadData.AddTexCoord(Vec2f{ 0.0f, 0.0f }); // bottom-left
		quadData.AddTexCoord(Vec2f{ 1.0f, 0.0f }); // bottom-right
		quadData.AddTexCoord(Vec2f{ 1.0f, 1.0f }); // top-right
		quadData.AddTexCoord(Vec2f{ 0.0f, 1.0f }); // top-left

		// Indices (two triangles)
		quadData.Indices = { 2, 1, 0, 0, 3, 2 };

		return quadData.Pack();
	}

	void ImportQuadMesh()
	{
		Mesh* quadMesh = TObject::Create<Mesh>(Collections::quadMeshName);
		quadMesh->SetData(Collections::CreateQuadPackedData(), MeshDrawType::STATIC);

		Application::GetInstance().GetService<AssetsService>()->AddBuiltIn(Collections::quadMeshUUID, quadMesh);

	}
}