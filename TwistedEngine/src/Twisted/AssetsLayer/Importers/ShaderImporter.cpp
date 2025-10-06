#include "Twisted/AssetsLayer/ImporterRegistry.h"  // <-- declares Twisted::ImporterRegistry
#include "ShaderImporter.h"
#include "TwistedMacros.h"    // <-- defines REGISTER_IMPORTER

#include "Twisted/Rendering/Shader.h"
#include "Utils/FileUtils.h"
#include "Utils/Utils.h"

#include "Utils/WPtr.h"
#include "Logger.h"

namespace Twisted
{
	ShaderData LoadData(const fs::path& assetPath)
	{
		ShaderData shaderData;

		std::string shaderText = Utils::ReadFileContent(assetPath);
		std::vector<std::string> shaderCodes = Utils::SplitString(shaderText, SHADER_DELIMITER);

		if (shaderCodes.size() != 2)
		{
			TWISTED_WARN("Invalid shader file: " + assetPath.string());
			return shaderData;
		}

		shaderData.VertShader = shaderCodes[0];
		shaderData.FragShader = shaderCodes[1];
		shaderData.GeoShader = "";

		return shaderData;
	}

	std::vector<TObject*>& ShaderImporter::Import(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const
	{
		ShaderData shaderData = LoadData(assetInfo.AssetPath);

		if (objects.size() > 0)
		{
			Shader* shader = objects[0]->static_as<Shader>();

			shader->Clear();
			shader->Init(shaderData);
		}	
		else
			objects.emplace_back(TObject::Create<Shader>(shaderData));

		return objects;
	}

	void ShaderImporter::PostImport(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const
	{

	}
}

REGISTER_IMPORTER(ShaderImporter)
