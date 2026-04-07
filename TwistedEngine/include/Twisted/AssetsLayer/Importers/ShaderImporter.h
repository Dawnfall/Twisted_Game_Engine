#pragma once
#include "AppCore.h"
#include "Twisted/AssetsLayer/AssetImporter.h"

#include <filesystem>
#include <string>
#include <vector>

namespace Twisted
{
	const std::string ASSET_SHADER_TYPE = "shader";
	const std::string SHADER_DELIMITER = "//---//";

	class TWISTED_API ShaderImporter : public AssetImporter
	{
	public:
		std::vector<WPtrBase> Load(const fs::path& path) const override;
		void HotReload(const fs::path& path, std::vector<WPtrBase>& objects) const override;

		std::string GetAssetType() const override { return ASSET_SHADER_TYPE; }
		bool DoAutoImport() const override { return true; }
		std::string GetCreatePath() const override { return "Shader"; }
		std::string DefaultFileName() const override { return "newShader.shader"; }
		std::vector<fs::path> GetAssetExtensions() const override { return { ".shader" }; }
	};
}
