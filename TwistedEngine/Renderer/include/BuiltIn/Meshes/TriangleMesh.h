#pragma once

#include "Utils/GlmUtils.h"
#include "Mesh.h"
#include "Data/MeshData.h"

#include "AssetUuid.h"
#include "AssetsService.h"
namespace Twisted::Collections
{
	constexpr AssetUuid triangleMeshUUID{ 0x9B3D3E56D2414C2CULL, 0xA583EFB9E6C23F71ULL };
	inline constexpr const char* triangleMeshName = "triangle";
	inline PackedMeshData CreateTrianglePackedData()
	{
		MeshData triangleData;
		triangleData.AddPosition(Vec3f{ -0.5f, -0.5f, 0.0f });
		triangleData.AddPosition(Vec3f{ 0.5f, -0.5f, 0.0f });
		triangleData.AddPosition(Vec3f{ 0.0f,  0.5f, 0.0f });

		triangleData.Indices = { 0,1,2 };
		return triangleData.Pack();
	}

	inline void ImportTriangleMesh()
	{
		Mesh* triangleMesh = TObject::Create<Mesh>(Collections::triangleMeshName);
		triangleMesh->SetData(Collections::CreateTrianglePackedData(), MeshDrawType::STATIC);
		Application::GetInstance().GetService<AssetsService>()->AddBuiltIn(Collections::triangleMeshUUID, triangleMesh);
	}
}
