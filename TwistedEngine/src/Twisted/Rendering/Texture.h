#pragma once
#include "AppCore.h"
#include "Twisted/Data/TextureData.h"

//#include <stbi_image/stb_image.h>

namespace Twisted::Render
{


	struct TWISTED_API Texture
	{
		Texture(const std::string& name, unsigned int textureID) :
			Name(name),
			TextureID(textureID)
		{
		}
		~Texture();

		std::string Name;
		unsigned int TextureID;

		static SRef<Texture> CreateTexture(const TextureData& texData);
	};
}