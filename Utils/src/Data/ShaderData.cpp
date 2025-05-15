#include "ShaderData.h"
#include "Utils/FileUtils.h"
#include "Utils/Utils.h"

namespace Twisted
{
	URef<ShaderData> ShaderData::ImportShaderData(const std::filesystem::path& assetPath)
	{
		std::string shaderText = Utils::ReadFileContent(assetPath);
		std::vector<std::string> shaderCodes = Utils::SplitString(shaderText, SHADER_DELIMITER);

		if (shaderCodes.size() != 2)
		{
			TWISTED_WARN("Invalid shader file: " + assetPath.string());
			return nullptr;
		}

		URef<ShaderData> shaderData;
		shaderData->Name = assetPath.stem().string();
		shaderData->VertShaderCode = shaderCodes[0];
		shaderData->FragShaderCode = shaderCodes[1];

		return shaderData;
	}
}