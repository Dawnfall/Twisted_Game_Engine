#pragma once

#include "AppCore.h"
#include "AppCore.h"

#include <filesystem>

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
		~TextureData();

		std::string Name;
		int Width;
		int Height;
		int NrChannels;
		unsigned char* Data;

		static URef<TextureData> ImportTextureData(const std::filesystem::path& assetPath);
	};
}