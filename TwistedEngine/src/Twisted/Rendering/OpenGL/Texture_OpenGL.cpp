#include "Twisted/Rendering/Texture.h"

#include "Logger.h"
#include <glad/glad.h>

namespace Twisted
{
	static GLenum WrapToGL(TextureWrap wrap)
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
	static GLenum MagFilterToGL(TextureMagFilter magFilter)
	{
		switch (magFilter)
		{
		case TextureMagFilter::NEAREST:
			return GL_NEAREST;
		case TextureMagFilter::LINEAR:
			return GL_LINEAR;
		default:
			TWISTED_ERROR("Unsupported texture magnification filter");
			return GL_NEAREST;
		}
	}
	static GLenum MinFilterToGL(TextureMinFilter minFilter)
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

	void Texture::Create(const TextureData& texData, const TextureParams& params)
	{
		m_params = params;

		m_width = texData.Width;
		m_height = texData.Height;
		m_channels = texData.Channels;

		m_isDirty = true;
		glGenTextures(1, &m_texID);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.Data); //input must always be RGBA;4 channels
		Update();
	}

	void Texture::Update()
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapToGL(m_params.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapToGL(m_params.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterToGL(m_params.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterToGL(m_params.MagFilter));

		if (m_params.DoMipMaps)
			glGenerateMipmap(GL_TEXTURE_2D);

		m_isDirty = false;
	}

	void Texture::Clear()
	{
		glDeleteTextures(1, &m_texID);
		m_texID = 0;
		m_width = m_height = m_channels = 0;
	}

	void Texture::UnBind()const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Bind(unsigned int slot)const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_texID);
	}
}