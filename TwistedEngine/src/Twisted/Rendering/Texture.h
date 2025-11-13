#pragma once
#include "AppCore.h"
#include "Twisted/TObject.h"
#include <string>
#include "Utils/GlmUtils.h"

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
		Texture(const std::string& name):
			TObject(name)
		{}
		Texture(const std::string& name,const TextureParams& params) :
			TObject(name),
			m_params(params)
		{}

		unsigned int GetTexID() const { return m_texID; }
		const TextureParams& GetParams() const { return m_params; }
		int GetWidth()const { return m_width; }
		int GetHeight()const { return m_height; }
		int GetChannelsCount()const { return m_channels; }
		bool IsValid()const { return m_texID != 0; }

		void Clear();

		void OnDestroy() override
		{
			Clear();
		}

		void SetData(TextureData& data);
		void Resize(const Vec2i& newSize);
		void Bind(unsigned int slot = 0) const;
		void UnBind()const;

		void SetParams(const TextureParams& params);
		void SetWrapType(TextureWrap wrapType);
		void SetMagFilter(TextureMagFilter magFilter);
		void SetMinFilter(TextureMinFilter minFilter);

	private:
		void ApplyParams()const;

		TextureParams m_params;

		int m_width = 0;
		int m_height = 0;
		int m_channels = 0;
		unsigned int m_texID = 0;
	};
}

