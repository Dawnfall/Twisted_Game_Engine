#include "Twisted/Rendering/Texture.h"

#include "Debug/Logger.h"
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

	void Texture::SetData(TextureData& data)
	{
		Clear();

		m_width = data.Width;
		m_height = data.Height;
		m_channels = data.Channels;

		glGenTextures(1, &m_texID);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			m_width,
			m_height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data.Data);

		ApplyParams();
		
		glBindTexture(GL_TEXTURE_2D, 0);

		//currently input must always be RGBA;4 channels

	}

	void Texture::Resize(const Vec2i& newSize)
	{
		if (newSize.x <= 0 || newSize.y <= 0)
			return; // dont allow zero or negative sizes

		if (m_width == newSize.x && m_height == newSize.y && IsValid())
			return; // already correct size

		m_width = newSize.x;
		m_height = newSize.y;

		if (!IsValid())
		{
			glGenTextures(1, &m_texID);
		}

		Bind();

		glTexImage2D(
			GL_TEXTURE_2D,
			0, //mipamap
			GL_RGBA, //format
			m_width,
			m_height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			nullptr
		);

		ApplyParams();

		UnBind();
	}

	void Texture::SetParams(const TextureParams& params)
	{
		m_params = params;

		if (!IsValid())
			return;

		Bind();
		ApplyParams();
		UnBind();
	}

	void Texture::SetWrapType(TextureWrap wrapType)
	{
		if (m_params.Wrap == wrapType)
			return;

		m_params.Wrap = wrapType;

		if (!IsValid())
			return;

		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapToGL(m_params.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapToGL(m_params.Wrap));
		UnBind();
	}

	void Texture::SetMagFilter(TextureMagFilter magFilter)
	{
		if (m_params.MagFilter == magFilter)
			return;

		m_params.MagFilter = magFilter;

		if (!IsValid())
			return;

		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterToGL(m_params.MagFilter));
		UnBind();
	}

	void Texture::SetMinFilter(TextureMinFilter minFilter)
	{
		if (m_params.MinFilter == minFilter)
			return;

		m_params.MinFilter = minFilter;

		if (!IsValid())
			return;

		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterToGL(m_params.MinFilter));
		if (m_params.DoMipMaps)
			glGenerateMipmap(GL_TEXTURE_2D);
		UnBind();
	}

	void Texture::Clear()
	{
		if (!IsValid())
			return;

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

	void Texture::ApplyParams()const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapToGL(m_params.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapToGL(m_params.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterToGL(m_params.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterToGL(m_params.MagFilter));

		if (m_params.DoMipMaps)
			glGenerateMipmap(GL_TEXTURE_2D);
	}
}

