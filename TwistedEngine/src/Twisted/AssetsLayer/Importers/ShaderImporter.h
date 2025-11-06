#pragma once
#include "Twisted/AssetsLayer/AssetImporter.h"

namespace Twisted
{
	const std::string ASSET_SHADER_TYPE = "shader";
	const std::string SHADER_DELIMITER = "//---//";

	class TWISTED_API ShaderImporter :public AssetImporter
	{
	public:

		void ImportNew(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void HotReload(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void PostImport(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void FillDefaultInfo(YAML::Node& node)const override
		{
			node[ASSET_TYPE_KEY] = ASSET_SHADER_TYPE;
		}

		inline bool DoAutoImport()const override { return true; }
		void CreateNewAsset(const fs::path& path) const override {}
		std::string GetCreatePath()const override { return "Shader"; }
		std::string DefaultFileName()const override { return "newShader.shader"; }
		std::vector<fs::path> GetAssetExtensions()const override {return { ".shader" };}
	};
}

