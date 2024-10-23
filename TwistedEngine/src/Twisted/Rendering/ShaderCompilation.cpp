#include "pch.h"
#include "ShaderCompilation.h"
#include "Twisted/Resources/ShaderData.h"
#include "Shader.h"
#include "Utils/Utils.h"

#include "OpenGL/Render_OpenGL.h"

namespace Twisted::ShaderCompilation
{
	std::shared_ptr<Shader> LoadShader(const std::string& fullPath, const std::string& shaderName)
	{
		std::string shaderText = Utils::ReadFileContent(fullPath);
		std::vector<std::string> shaderCodes = Utils::SplitString(shaderText, SHADER_DELIMITER);
		if (shaderCodes.size() != 2)
		{
			TWISTED_WARN("Invalid shader file: " + fullPath);
			return nullptr;
		}

		ShaderData shaderData;
		shaderData.Name = shaderName;
		shaderData.VertShaderCode = shaderCodes[0];
		shaderData.FragShaderCode = shaderCodes[1];

		return Render_OpenGL::CreateShader(shaderData);
	}
}