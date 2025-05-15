#pragma once
#include "AppCore.h"
#include "Data/TextureData.h"

namespace Twisted
{
	//enum class TextureType
	//{
	//	TEXTURE2D,
	//	CUBEMAP
	//};

	struct Texture
	{
		Texture(const std::string& name, unsigned int textureID) :
			Name(name),
			TextureID(textureID)
		{
		}
		~Texture();

		//TextureType Type;
		std::string Name;
		unsigned int TextureID;

		static SRef<Texture> CreateTexture(const TextureData& texData);
	};
}