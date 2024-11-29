#pragma once

#include "twistedpch.h"
#include "AppCore.h"
namespace Twisted
{
	//enum class TextureType
	//{
	//	TEXTURE2D,
	//	CUBEMAP
	//};

	struct TWISTED_API Texture
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
	};
}