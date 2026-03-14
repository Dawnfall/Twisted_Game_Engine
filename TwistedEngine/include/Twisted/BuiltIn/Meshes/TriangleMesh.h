#pragma once

#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Data/MeshData.h"

#include "Twisted/AssetsLayer/AssetUuid.h"
#include "Twisted/AssetsLayer/AssetsService.h"
namespace Twisted::Collections
{
	constexpr AssetUuid triangleMeshUUID{ 0x9B3D3E56D2414C2CULL, 0xA583EFB9E6C23F71ULL };
	const std::string triangleMeshName = "triangle";
	inline PackedMeshData CreateTrianglePackedData()
	{
		MeshData triangleData;
		triangleData.AddPosition(Vec3f{ -0.5f, -0.5f, 0.0f });
		triangleData.AddPosition(Vec3f{ 0.5f, -0.5f, 0.0f });
		triangleData.AddPosition(Vec3f{ 0.0f,  0.5f, 0.0f });

		triangleData.Indices = { 2,1,0 };
		return triangleData.Pack();
	}

	void ImportTriangleMesh()
	{
		Mesh* triangleMesh = TObject::Create<Mesh>(Collections::triangleMeshName);
		triangleMesh->SetData(Collections::CreateTrianglePackedData(), MeshDrawType::STATIC);
		Application::GetInstance().GetService<AssetsService>()->AddBuiltIn(Collections::triangleMeshUUID, triangleMesh);
	}
}