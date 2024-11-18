#pragma once

#include "AppCore.h"
#include "twistedpch.h"
#include "Asset.h"

namespace Twisted
{
	struct TWISTED_API TextureData :public Asset
	{
		~TextureData()
		{
			stbi_image_free(data);
		}

		int Width;
		int Height;
		int NrChannels;
		unsigned char* data;
	};
}