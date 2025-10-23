#pragma once
#include "AppCore.h"
#include "Twisted/TObject.h"
#include <string>

namespace Twisted
{
	enum class TextureWrap
	{
		REPEAT,
		MIRROR_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};
	enum class TextureMinFilter
	{
		NEAREST,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_NEAREST,
		LINEAR_MIPMAP_LINEAR
	};
	enum class TextureMagFilter
	{
		NEAREST,
		LINEAR
	};

	struct TextureParams
	{
		TextureWrap Wrap = TextureWrap::REPEAT;
		TextureMinFilter MinFilter = TextureMinFilter::NEAREST;
		TextureMagFilter MagFilter = TextureMagFilter::NEAREST;
		bool DoMipMaps = false;
	};

	struct TextureData
	{
		unsigned char* Data;
		int Width;
		int Height;
		int Channels;
	};

	class TWISTED_API Texture :public TObject //assumes mipmap, rgba 4 channel input , assumes valid texture
	{
	public:
		Texture() = default;
		Texture(const TextureData& texData, const TextureParams& params) :TObject()
		{
			Create(texData, params);
		}
		~Texture()
		{
			Clear();
		}

		void Create(const TextureData& data, const TextureParams& params);
		void Clear();

		unsigned int GetTexID() const { return m_texID; }
		const TextureParams& GetParams() const { return m_params; }
		int GetWidth()const { return m_width; }
		int GetHeight()const { return m_height; }
		int GetChannelsCount()const { return m_channels; }

		void SetParams(const TextureParams& params)
		{
			m_params = params;
			m_isDirty = true;
		}
		void SetWrapType(TextureWrap wrapType)
		{
			if (m_params.Wrap == wrapType)
				return;
			m_params.Wrap = wrapType;
			m_isDirty = true;
		}
		void SetMagFilter(TextureMagFilter magFilter)
		{
			if (m_params.MagFilter == magFilter)
				return;
			m_params.MagFilter = magFilter;
			m_isDirty = true;
		}
		void SetMinFilter(TextureMinFilter minFilter)
		{
			if (m_params.MinFilter == minFilter)
				return;
			m_params.MinFilter = minFilter;
			m_isDirty = true;
		}

		void Bind(unsigned int slot = 0) const;
		void UnBind()const;

		void Update();

	private:

		TextureParams m_params;
		bool m_isDirty = true;

		int m_width = 0;
		int m_height = 0;
		int m_channels = 0;
		unsigned int m_texID = 0;
	};
}