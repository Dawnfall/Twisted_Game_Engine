#ifdef TWISTED_BACKEND_OPENGL

#include "Texture.h"
#include "OpenGL/Texture_OpenGL.h"
#include "Data/TextureData.h"
#include "Debug/Logger.h"
#include <string>

namespace Twisted
{
	Texture::Texture(const std::string& name) :
		TObject(name),
		m_backend(new TextureBackend())
	{
	}
	Texture::~Texture()
	{
		delete m_backend;
	}

	void Texture::SetData(const TextureData& data, TextureParams params)
	{
		Clear();
		Params = params;

		Info.Width = data.Width;
		Info.Height = data.Height;
		Info.Format = TextureFormat::RGBA8; // data.Channels;

		Info.MipLevels = Params.DoMipMaps ? (uint32_t)GL::CalcMipCount2D(Info.Width, Info.Height) : 1;

		// (Re)create texture object if needed
		if (!m_backend->IsValid())
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_backend->TexID);
			Version++;
		}

		// Allocate immutable storage every time SetData is called.
		// If you want to avoid reallocations on repeated uploads of same size/format, add cached fields and compare.
		const GLenum internalFmt = GL::ToGLInternalFormat(Info.Format);
		glTextureStorage2D(m_backend->TexID, (GLint)Info.MipLevels, internalFmt, Info.Width, Info.Height);
		Version++; // storage was (re)allocated

		// Apply sampler state (stored on the texture here)
		glTextureParameteri(m_backend->TexID, GL_TEXTURE_WRAP_S, GL::WrapToGL(Params.Wrap));
		glTextureParameteri(m_backend->TexID, GL_TEXTURE_WRAP_T, GL::WrapToGL(Params.Wrap));
		glTextureParameteri(m_backend->TexID, GL_TEXTURE_MIN_FILTER, GL::MinFilterToGL(Params.MinFilter));
		glTextureParameteri(m_backend->TexID, GL_TEXTURE_MAG_FILTER, GL::MagFilterToGL(Params.MagFilter));

		// Upload base level
		GLenum extFmt = GL_RGBA, extType = GL_UNSIGNED_BYTE;
		GL::ToGLExternalFormatAndType(Info.Format, extFmt, extType);

		// Optional: alignment (RGBA8 is usually fine with 4; set if you have tightly packed data)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTextureSubImage2D(
			m_backend->TexID,
			0,
			0, 0,
			Info.Width, Info.Height,
			extFmt,
			extType,
			data.Data
		);

		if (Params.DoMipMaps && Info.MipLevels > 1)
			glGenerateTextureMipmap(m_backend->TexID);
	}


	void Texture::Resize(const Vec2i& newSize)
	{
		if (newSize.x <= 0 || newSize.y <= 0)
			return; // dont allow zero or negative sizes

		if (Info.Width == newSize.x && Info.Height == newSize.y && m_backend->IsValid())
			return; // already correct size

		glDeleteTextures(1, &m_backend->TexID);
		m_backend->TexID = 0;

		Info.Width = newSize.x;
		Info.Height = newSize.y;

		Info.MipLevels = Params.DoMipMaps ? (uint32_t)GL::CalcMipCount2D(Info.Width, Info.Height) : 1;

		if (!m_backend->IsValid())
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_backend->TexID);
			++Version;
		}

		// Allocate immutable storage every time SetData is called.
		// If you want to avoid reallocations on repeated uploads of same size/format, add cached fields and compare.
		const GLenum internalFmt = GL::ToGLInternalFormat(Info.Format);
		glTextureStorage2D(m_backend->TexID, (GLint)Info.MipLevels, internalFmt, Info.Width, Info.Height);
		Version++; // storage was (re)allocated

		// Apply sampler state (stored on the texture here)
		glTextureParameteri(m_backend->TexID, GL_TEXTURE_WRAP_S, GL::WrapToGL(Params.Wrap));
		glTextureParameteri(m_backend->TexID, GL_TEXTURE_WRAP_T, GL::WrapToGL(Params.Wrap));
		glTextureParameteri(m_backend->TexID, GL_TEXTURE_MIN_FILTER, GL::MinFilterToGL(Params.MinFilter));
		glTextureParameteri(m_backend->TexID, GL_TEXTURE_MAG_FILTER, GL::MagFilterToGL(Params.MagFilter));

		// Upload base level
		GLenum extFmt = GL_RGBA;
		GLenum extType = GL_UNSIGNED_BYTE;
		GL::ToGLExternalFormatAndType(Info.Format, extFmt, extType);

		// Optional: alignment (RGBA8 is usually fine with 4; set if you have tightly packed data)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if (Params.DoMipMaps && Info.MipLevels > 1)
			glGenerateTextureMipmap(m_backend->TexID);
	}

	void Texture::Clear()
	{
		if (!m_backend->IsValid())
			return;

		glDeleteTextures(1, &m_backend->TexID);
		m_backend->TexID = 0;
		Info.Width = Info.Height = 0;
		Info.MipLevels = 0;
		Version++;
	}

	void Texture::OnDestroy()
	{
		Clear();
	}
}


namespace Twisted::GL
{
	void Bind(GLuint texID,GLenum slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texID);
	}

	void UnBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLenum WrapToGL(TextureWrap wrap)
	{
		switch (wrap)
		{
		case Twisted::TextureWrap::REPEAT:
			return GL_REPEAT;
		case Twisted::TextureWrap::MIRROR_REPEAT:
			return GL_MIRRORED_REPEAT;
		case Twisted::TextureWrap::CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		case Twisted::TextureWrap::CLAMP_TO_BORDER:
			return GL_CLAMP_TO_BORDER;
		default:
			TWISTED_ERROR("Unsupported texture wrap type");
			return GL_REPEAT;
		}
	}

	GLenum MagFilterToGL(TextureMagFilter magFilter)
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

	GLenum MinFilterToGL(TextureMinFilter minFilter)
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

	GLenum ToGLInternalFormat(TextureFormat fmt)
	{
		switch (fmt)
		{
		case TextureFormat::RGBA8:            return GL_RGBA8;
		case TextureFormat::RGBA16F:          return GL_RGBA16F;
		case TextureFormat::RGBA32F:          return GL_RGBA32F;
		case TextureFormat::Depth24Stencil8:  return GL_DEPTH24_STENCIL8;
		default: return GL_RGBA8;
		}
	}

	void ToGLExternalFormatAndType(TextureFormat fmt, GLenum& outFormat, GLenum& outType)
	{
		// For uploads. Extend for other formats as needed.
		switch (fmt)
		{
		case TextureFormat::RGBA8:
			outFormat = GL_RGBA;
			outType = GL_UNSIGNED_BYTE;
			return;
		case TextureFormat::RGBA16F:
			outFormat = GL_RGBA;
			outType = GL_HALF_FLOAT;
			return;
		case TextureFormat::RGBA32F:
			outFormat = GL_RGBA;
			outType = GL_FLOAT;
			return;
		default:
			outFormat = GL_RGBA;
			outType = GL_UNSIGNED_BYTE;
			return;
		}
	}

	int CalcMipCount2D(int w, int h)
	{
		int m = std::max(w, h);
		int levels = 1;
		while (m > 1) 
		{ m >>= 1; ++levels; }
		return levels;
	}

	void SetMagFilter([[maybe_unused]] GLuint tex, [[maybe_unused]] TextureMagFilter& magFilter)
	{
		//if (tex.Params.MagFilter == magFilter)
		//	return;

		//tex.Params.MagFilter = magFilter;

		//if (!tex.IsValid())
		//	return;

		//tex.UnBind();
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL::MagFilterToGL(tex.Params.MagFilter));
		//tex.UnBind();
	}

	void SetParams([[maybe_unused]] GLuint tex, [[maybe_unused]] const TextureParams& params)
	{
		//tex.Params = params;

		//if (!tex.IsValid())
		//	return;

		//tex.UnBind();
		//GL::ApplyParams(tex.Params);
		//tex.UnBind();
	}

	void SetMinFilter([[maybe_unused]] GLuint tex, [[maybe_unused]] TextureMinFilter minFilter)
	{
		//if (tex.Params.MinFilter == minFilter)
		//	return;

		//tex.Params.MinFilter = minFilter;

		//if (!tex.IsValid())
		//	return;

		//tex.UnBind();
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL::MinFilterToGL(tex.Params.MinFilter));
		//if (tex.Params.DoMipMaps)
		//	glGenerateMipmap(GL_TEXTURE_2D);
		//tex.UnBind();
	}

	void SetWrapType([[maybe_unused]] GLuint tex, [[maybe_unused]] TextureWrap wrapType)
	{
		//if (tex.Params.Wrap == wrapType)
		//	return;

		//tex.Params.Wrap = wrapType;

		//if (!tex.IsValid())
		//	return;

		//tex.UnBind();
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL::WrapToGL(tex.Params.Wrap));
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL::WrapToGL(tex.Params.Wrap));
		//tex.UnBind();
	}

	void ApplyParams(const TextureParams& params) //assumes bound and valid
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapToGL(params.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapToGL(params.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterToGL(params.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterToGL(params.MagFilter));

		if (params.DoMipMaps)
			glGenerateMipmap(GL_TEXTURE_2D);
	}
}


#endif