#pragma once

#include "twistedpch.h"
namespace Twisted
{
	//enum class TextureType
	//{
	//	TEXTURE2D,
	//	CUBEMAP
	//};
	struct TextureData
	{
		int Width;
		int Height;
		int NrChannels;
		unsigned char* data;
	};

	struct Texture
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