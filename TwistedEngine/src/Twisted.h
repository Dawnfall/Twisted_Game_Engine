#pragma once
#include "AppCore.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"

namespace rend = Twisted::Render;

namespace Twisted
{
	bool TWISTED_API Init();
	void TWISTED_API Terminate();

	URef<rend::ShaderData> TWISTED_API ImportShaderData(const std::filesystem::path& assetPath);
	URef<rend::TextureData> TWISTED_API ImportTextureData(const std::filesystem::path& assetPath);

}