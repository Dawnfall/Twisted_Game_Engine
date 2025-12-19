#pragma once
#include "Twisted/AssetsLayer/AssetImporter.h"
#include "AppCore.h"
#include <yaml-cpp/node/node.h>
#include <filesystem>
#include <string>
#include <vector>
#include "Twisted/AssetsLayer/AssetInfo.h"
#include "Utils/WPtrBase.h"

namespace Twisted
{
	const std::string ASSET_SHADER_TYPE = "shader";
	const std::string SHADER_DELIMITER = "//---//";

	class TWISTED_API ShaderImporter :public AssetImporter
	{
	public:

		void ImportNew(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void HotReload(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void PostImport(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void FillDefaultInfo(YAML::Node& node)const override
		{
			node[ASSET_TYPE_KEY] = ASSET_SHADER_TYPE;
		}

		inline bool DoAutoImport()const override { return true; }
		std::string GetCreatePath()const override { return "Shader"; }
		std::string DefaultFileName()const override { return "newShader.shader"; }
		std::vector<fs::path> GetAssetExtensions()const override {return { ".shader" };}
	};
}



