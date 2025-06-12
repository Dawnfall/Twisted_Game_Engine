#pragma once

#include <string>

namespace Twisted
{
	struct TextureData
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
}