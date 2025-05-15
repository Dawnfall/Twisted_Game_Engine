#pragma once
#include "AppCore.h"
#include <string>
#include <filesystem>

namespace Twisted
{
	const std::string SHADER_EXTENSION = ".shader";
	const std::string SHADER_DELIMITER = "---";

	struct ShaderData
	{
		std::string Name = "";
		std::string VertShaderCode = "";
		std::string FragShaderCode = "";
		
		bool IsEmpty() { return VertShaderCode == ""; }

		static URef<ShaderData> ImportShaderData(const std::filesystem::path& assetPath);
	};
}