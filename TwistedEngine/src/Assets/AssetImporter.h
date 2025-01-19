#pragma once

#include "Asset.h"
#include "Data/TextureData.h"
#include "Data/ShaderData.h"
#include "Data/MeshData.h"
#include "Data/MaterialData.h"

namespace Twisted
{
	namespace AssetImporter
	{
		SRef<Asset> ImportAsset(const std::filesystem::path& assetPath);
		SRef<ShaderData> ImportShaderData(const std::filesystem::path& assetPath);
		SRef<MeshData> ImportMeshData(const std::filesystem::path& assetPath);
		SRef<MaterialData> ImportMaterial(const std::filesystem::path& assetPath);
	};
}