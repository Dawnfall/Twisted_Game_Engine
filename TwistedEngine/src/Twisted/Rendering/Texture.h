#pragma once

#include <string>

namespace Twisted
{
	struct TextureData
	{
		int Width;
		int Height;
		int NrChannels;
		unsigned char* data;
	};

	struct Texture
	{
		//GLuint
		std::string Name;
		unsigned int ID;
	};
}