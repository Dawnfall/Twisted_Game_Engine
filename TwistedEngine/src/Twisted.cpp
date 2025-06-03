#include "twistedpch.h"
#include "Twisted.h"
#include "AppCore.h"
#include "Utils/FileUtils.h"
#include "Utils/Utils.h"

#include "Rendering/Shader.h"

#include <stbi_image/stb_image.h>
#include "Constants.h"
#include "Twisted/Application/Window.h"

namespace rend = Twisted::Render;

namespace Twisted
{
	bool Init()
	{
		if (Logger::Init() && InitGLFW())
		{
			Twisted::TWISTED_INFO("Engine Init Success!");
			return true;
		}

		Twisted::TWISTED_ERROR("Engine Init failure");
		return false;
	}

	void Terminate()
	{
		TerminateGLFW();
	}

	URef<rend::ShaderData> ImportShaderData(const std::filesystem::path& assetPath)
	{
		std::string shaderText = Utils::ReadFileContent(assetPath);
		std::vector<std::string> shaderCodes = Utils::SplitString(shaderText, SHADER_DELIMITER);

		if (shaderCodes.size() != 2)
		{
			TWISTED_WARN("Invalid shader file: " + assetPath.string());
			return nullptr;
		}

		URef<rend::ShaderData> shaderData;
		shaderData->Name = assetPath.stem().string();
		shaderData->VertShaderCode = shaderCodes[0];
		shaderData->FragShaderCode = shaderCodes[1];

		return shaderData;
	}

	URef<rend::TextureData> ImportTextureData(const std::filesystem::path& assetPath)
	{
		std::string ext = assetPath.extension().string();
		if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".tga")
		{
			int width, height, channels;
			unsigned char* data = stbi_load(assetPath.string().c_str(), &width, &height, &channels, 0);

			if (data != nullptr)
				return std::make_unique<rend::TextureData>(width, height, channels, data);
			return nullptr;
		}
		TWISTED_WARN("Unsuported texture format");
		return nullptr;
	}
}
