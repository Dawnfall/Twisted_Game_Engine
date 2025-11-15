#include "ShaderImporter.h"
#include "Twisted/Rendering/Shader.h"
#include "Utils/FileUtils.h"
#include "Utils/Utils.h"

#include "Utils/WPtr.h"
#include "Debug/Logger.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"

#include "Twisted/Rendering/OpenGL/Shader_OpenGL.h"

namespace Twisted
{

	ShaderData LoadData(const fs::path& assetPath)
	{
		ShaderData shaderData;

		std::string shaderText = Utils::ReadFileContent(assetPath);
		std::vector<std::string> shaderCodes = Utils::SplitString(shaderText, SHADER_DELIMITER);

		if (shaderCodes.size() != 2)
		{
			TWISTED_WARN(std::format("Invalid shader file: {}", assetPath.string()));
			return shaderData;
		}

		shaderData.VertShader = shaderCodes[0];
		shaderData.FragShader = shaderCodes[1];
		shaderData.GeoShader = "";

		return shaderData;
	}

	void ShaderImporter::ImportNew(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		ShaderData shaderData = LoadData(assetInfo.GetAssetPath());

		WPtr<Shader> shader(TObject::Create<Shader>(assetInfo.GetAssetName()));
		Shader_GL::SetData(*shader,shaderData);

		objects.emplace_back(shader);
	}

	void ShaderImporter::HotReload(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		//TODO:...
		//ShaderData shaderData = LoadData(assetInfo.AssetPath);
		//Shader* shader = objects[""].GetObj()->static_as<Shader>();

		//shader->Clear();
		//shader->Init(shaderData);
	}



	void ShaderImporter::PostImport(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{

	}
}

REGISTER_IMPORTER(ShaderImporter)


