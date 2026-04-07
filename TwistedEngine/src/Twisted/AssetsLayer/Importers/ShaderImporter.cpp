#include "Twisted/AssetsLayer/Importers/ShaderImporter.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"

#include "Twisted/Rendering/Shader.h"
#include "Twisted/Rendering/OpenGL/Shader_OpenGL.h"
#include "Twisted/TObject.h"
#include "Utils/WPtr.h"
#include "Utils/WPtrBase.h"
#include "Utils/FileUtils.h"
#include "Utils/Utils.h"
#include "Debug/Logger.h"

#include <filesystem>
#include <string>
#include <vector>
#include <format>

namespace Twisted
{
	static ShaderData LoadData(const fs::path& path)
	{
		ShaderData shaderData;

		std::string shaderText = Utils::ReadFileContent(path);
		std::vector<std::string> shaderCodes = Utils::SplitString(shaderText, SHADER_DELIMITER);

		if (shaderCodes.size() != 2)
		{
			TWISTED_WARN(std::format("Invalid shader file: {}", path.string()));
			return shaderData;
		}

		shaderData.VertShader = shaderCodes[0];
		shaderData.FragShader = shaderCodes[1];
		shaderData.GeoShader = "";

		return shaderData;
	}

	std::vector<WPtrBase> ShaderImporter::Load(const fs::path& path) const
	{
		ShaderData shaderData = LoadData(path);

		WPtr<Shader> shader(TObject::Create<Shader>(path.stem().string()));
		shader->SetData(shaderData);

		return { shader };
	}

	void ShaderImporter::HotReload(const fs::path& path, std::vector<WPtrBase>& objects) const
	{
		(void)path;
		(void)objects;
		//TODO...
	}
}

REGISTER_IMPORTER(ShaderImporter)
