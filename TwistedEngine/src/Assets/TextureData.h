#pragma once

#include "AppCore.h"
#include "twistedpch.h"
#include "Asset.h"
#include "Debug/Logger.h"

namespace Twisted
{
	struct TWISTED_API TextureData :public Asset
	{
		TextureData(int width, int height, int nrChannels, unsigned char* data) :
			Asset(),
			Width(width),
			Height(height),
			NrChannels(nrChannels),
			Data(data)
		{
		}

		~TextureData()
		{
			stbi_image_free(Data);
		}

		std::string Name;
		int Width;
		int Height;
		int NrChannels;
		unsigned char* Data;

		static SRef<TextureData> ImportTextureData(const std::filesystem::path& assetPath)
		{
			std::string ext = assetPath.extension().string();
			if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".tga")
			{
				int width, height, channels;
				unsigned char* data = stbi_load(assetPath.string().c_str(), &width, &height, &channels, 0);

				if (data != nullptr)
					return std::make_shared<TextureData>(width, height, channels, data);
				return nullptr;
			}
			TWISTED_WARN("Unsuported texture format");
			return nullptr;
		}
	};
}