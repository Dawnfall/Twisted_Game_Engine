#include "Twisted/Rendering/Texture.h"

#include "Logger.h"
#include <glad/glad.h>

namespace Twisted
{
	static int WrapToGL(TextureWrap wrap)
	{
		switch (wrap)
		{
		case TextureWrap::REPEAT:
			return GL_REPEAT;
		case TextureWrap::MIRROR_REPEAT:
			return GL_MIRRORED_REPEAT;
		case TextureWrap::CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		case TextureWrap::CLAMP_TO_BORDER:
			return GL_CLAMP_TO_BORDER;
		default:
			TWISTED_ERROR("Unsupported texture wrap type");
			return GL_REPEAT;
		}
	}
	static int MagFilterToGL(TextureMagFilter magFilter)
	{
		switch (magFilter)
		{
		case Twisted::TextureMagFilter::NEAREST:
			return GL_NEAREST;
		case Twisted::TextureMagFilter::LINEAR:
			return GL_LINEAR;
		default:
			TWISTED_ERROR("Unsupported texture magnification filter");
			return GL_NEAREST;
		}
	}
	static int MinFilterToGL(TextureMinFilter minFilter)
	{
		switch (minFilter)
		{
		case TextureMinFilter::NEAREST:
			return GL_NEAREST;
		case TextureMinFilter::LINEAR:
			return GL_LINEAR;
		case TextureMinFilter::NEAREST_MIPMAP_NEAREST:
			return GL_NEAREST_MIPMAP_NEAREST;
		case TextureMinFilter::NEAREST_MIPMAP_LINEAR:
			return GL_NEAREST_MIPMAP_LINEAR;
		case TextureMinFilter::LINEAR_MIPMAP_NEAREST:
			return GL_LINEAR_MIPMAP_NEAREST;
		case TextureMinFilter::LINEAR_MIPMAP_LINEAR:
			return GL_LINEAR_MIPMAP_LINEAR;
		default:
			TWISTED_ERROR("Unsupported texture minification filter");
			return GL_NEAREST;
		}
	}

	Texture::Texture(ObjectID id):
		BaseObject(id)
	{ }

	Texture::Texture(ObjectID id, const TextureData& texData) :
		BaseObject(id),
		m_width(texData.Width),
		m_height(texData.Height),
		m_channels(texData.Channels)
	{
		Init(texData);
	}

	void Texture::Init(const TextureData& data)
	{
		glGenTextures(1, &m_texID);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapToGL(m_wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapToGL(m_wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterToGL(m_minFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterToGL(m_magFilter));

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.Data); //input must always be RGBA;4 channels
		glGenerateMipmap(GL_TEXTURE_2D); //TODO.... should be optional
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_texID);
	}


	void Texture::SetWrapType(TextureWrap wrapType)
	{
		if (m_wrap == wrapType)
			return;
		m_wrap = wrapType;

		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapToGL(m_wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapToGL(m_wrap));
	}
	void Texture::SetMagFilter(TextureMagFilter magFilter)
	{
		if (m_magFilter == magFilter)
			return;
		m_magFilter = magFilter;

		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterToGL(m_magFilter));
	}
	void Texture::SetMinFilter(TextureMinFilter minFilter)
	{
		if (m_minFilter == minFilter)
			return;
		m_minFilter = minFilter;

		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterToGL(m_minFilter));
	}
}