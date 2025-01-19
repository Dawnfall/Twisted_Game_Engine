#include "renderingpch.h"

#include "Texture.h"
#include "RenderingAPI.h"


namespace Twisted
{
	Texture::~Texture()
	{
		glDeleteTextures(1, &TextureID);
	}

	SRef<Texture> Texture::CreateTexture(SRef<TextureData> texData)
	{
		GLuint id;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texData->Width, texData->Height, 0,/*(fmt==0)?GL_BGRA : GL_RGBA*/  GL_RGBA, GL_UNSIGNED_BYTE, texData->Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		SRef<Texture> newTex = std::make_shared<Texture>(texData->Name, id);
		//newTex->Type = TextureType::TEXTURE2D;

		return newTex;
	}
}