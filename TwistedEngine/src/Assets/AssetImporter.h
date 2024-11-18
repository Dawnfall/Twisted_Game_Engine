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
		std::shared_ptr<Asset> ImportAsset(const std::filesystem::path& assetPath);
		std::shared_ptr<TextureData> ImportTextureData(const std::filesystem::path& assetPath);
		std::shared_ptr<ShaderData> ImportShaderData(const std::filesystem::path& assetPath);
		std::shared_ptr<MeshData> ImportMeshData(const std::filesystem::path& assetPath);
		std::shared_ptr<MaterialData> ImportMaterial(const std::filesystem::path& assetPath);
	};
}