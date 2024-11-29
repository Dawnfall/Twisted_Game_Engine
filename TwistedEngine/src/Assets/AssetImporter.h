#pragma once

#include "twistedpch.h"
#include "Asset.h"
#include "TextureData.h"
#include "ShaderData.h"
#include "MeshData.h"
#include "MaterialData.h"

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