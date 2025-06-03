#pragma once
#include "AppCore.h"
//#include <stbi_image/stb_image.h>

namespace Twisted::Render
{
	struct TWISTED_API TextureData
	{
		TextureData(int width, int height, int nrChannels, unsigned char* data) :
			Width(width),
			Height(height),
			NrChannels(nrChannels),
			Data(data)
		{
		}
		~TextureData()
		{
			//TODO...stbi_image_free(Data);
		}

		std::string Name;
		int Width;
		int Height;
		int NrChannels;
		unsigned char* Data;

	};

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