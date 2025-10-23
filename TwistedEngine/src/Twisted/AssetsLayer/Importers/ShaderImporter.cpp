#include "Twisted/AssetsLayer/AssetsRegistry.h" 
#include "ShaderImporter.h"
#include "Twisted/TwistedMacros.h" 

#include "Twisted/Rendering/Shader.h"
#include "Utils/FileUtils.h"
#include "Utils/Utils.h"

#include "Utils/WPtr.h"
#include "Debug/Logger.h"

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

	ObjectsPerAsset& ShaderImporter::Import(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const
	{
		ShaderData shaderData = LoadData(assetInfo.AssetPath);

		if (objects.size() > 0)
		{
			Shader* shader = objects[""].GetObj()->static_as<Shader>();

			shader->Clear();
			shader->Init(shaderData);
		}	
		else
			objects[""]=WPtr<Shader>(TObject::Create<Shader>(shaderData));

		return objects;
	}

	void ShaderImporter::PostImport(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const
	{

	}
}

REGISTER_IMPORTER(ShaderImporter)
