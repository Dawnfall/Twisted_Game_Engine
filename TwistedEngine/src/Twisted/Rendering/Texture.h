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
		//Texture(const std::string& name, TextureType type, unsigned int textureID) :
		//	Name(name),
		//	Type(type),
		//	TextureID(textureID)
		//{}

		std::string Name;
		//TextureType Type;
		unsigned int TextureID;
	};
}