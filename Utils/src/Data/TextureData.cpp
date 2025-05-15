#include "TextureData.h"
#include "AppCore.h"

#include <stbi_image/stb_image.h>

namespace Twisted
{
	TextureData::~TextureData()
	{
		stbi_image_free(Data);
	}

	URef<TextureData> TextureData::ImportTextureData(const std::filesystem::path& assetPath)
	{
		std::string ext = assetPath.extension().string();
		if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".tga")
		{
			int width, height, channels;
			unsigned char* data = stbi_load(assetPath.string().c_str(), &width, &height, &channels, 0);

			if (data != nullptr)
				return std::make_unique<TextureData>(width, height, channels, data);
			return nullptr;
		}
		TWISTED_WARN("Unsuported texture format");
		return nullptr;
	}

}