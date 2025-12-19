#include "Twisted/Rendering/Texture.h"
#include "Twisted/Rendering/OpenGL/Texture_OpenGL.h"

#include "Debug/Logger.h"
#include <string>

namespace Twisted
{
	Texture::Texture(const std::string& name) :
		TObject(name)
	{
	}
	Texture::Texture(const std::string& name, const TextureParams& params) :
		TObject(name),
		Params(params)
	{
	}

	void Texture::OnDestroy()
	{
		Texture_GL::Clear(*this);
	}
}


namespace Twisted::Texture_GL
{
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

	void Clear(Texture& tex)
	{
		if (!tex.IsValid())
			return;

		glDeleteTextures(1, &tex.TexID);
		tex.TexID = 0;
		tex.Width = tex.Height = tex.Channels = 0;
	}

	void SetData(Texture& tex, TextureData& data)
	{
		Texture_GL::Clear(tex);

		tex.Width = data.Width;
		tex.Height = data.Height;
		tex.Channels = data.Channels;

		glGenTextures(1, &tex.TexID);
		glBindTexture(GL_TEXTURE_2D, tex.TexID);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA8,
			tex.Width,
			tex.Height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data.Data);

		Texture_GL::ApplyParams(tex.Params);

		glBindTexture(GL_TEXTURE_2D, 0);

		//currently input must always be RGBA;4 channels
	}

	void Resize(Texture& tex, const Vec2i& newSize)
	{
		if (newSize.x <= 0 || newSize.y <= 0)
			return; // dont allow zero or negative sizes

		if (tex.Width == newSize.x && tex.Height == newSize.y && tex.IsValid())
			return; // already correct size

		tex.Width = newSize.x;
		tex.Height = newSize.y;

		if (!tex.IsValid())
		{
			glGenTextures(1, &tex.TexID);
		}

		Texture_GL::Bind(tex, 0);//TODO...

		glTexImage2D(
			GL_TEXTURE_2D,
			0, //mipamap
			GL_RGBA, //format
			tex.Width,
			tex.Height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			nullptr
		);

		Texture_GL::ApplyParams(tex.Params);

		Texture_GL::UnBind();
	}

	void SetMagFilter(Texture tex, TextureMagFilter magFilter)
	{
		if (tex.Params.MagFilter == magFilter)
			return;

		tex.Params.MagFilter = magFilter;

		if (!tex.IsValid())
			return;

		Texture_GL::Bind(tex, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Texture_GL::MagFilterToGL(tex.Params.MagFilter));
		Texture_GL::UnBind();
	}

	void SetParams(Texture& tex, const TextureParams& params)
	{
		tex.Params = params;

		if (!tex.IsValid())
			return;

		Texture_GL::Bind(tex, 0); //TODO:... not sure
		Texture_GL::ApplyParams(tex.Params);
		Texture_GL::UnBind();
	}

	void SetMinFilter(Texture& tex, TextureMinFilter minFilter)
	{
		if (tex.Params.MinFilter == minFilter)
			return;

		tex.Params.MinFilter = minFilter;

		if (!tex.IsValid())
			return;

		Texture_GL::Bind(tex, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture_GL::MinFilterToGL(tex.Params.MinFilter));
		if (tex.Params.DoMipMaps)
			glGenerateMipmap(GL_TEXTURE_2D);
		Texture_GL::UnBind();
	}

	void SetWrapType(Texture& tex, TextureWrap wrapType)
	{
		if (tex.Params.Wrap == wrapType)
			return;

		tex.Params.Wrap = wrapType;

		if (!tex.IsValid())
			return;

		Texture_GL::Bind(tex, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Texture_GL::WrapToGL(tex.Params.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Texture_GL::WrapToGL(tex.Params.Wrap));
		Texture_GL::UnBind();
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

	void Bind(const Texture& tex, unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex.TexID);
	}

	void UnBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


