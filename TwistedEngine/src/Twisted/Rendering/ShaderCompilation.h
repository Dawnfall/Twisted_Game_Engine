#pragma once
#include "twistedpch.h"

#include "Shader.h"

namespace Twisted::ShaderCompilation
{
	const std::string SHADER_EXTENSION = ".shader";
	const std::string SHADER_DELIMITER = "---";

	std::shared_ptr<Shader> LoadShader(const std::string& fullPath,const std::string& shaderName);
}
