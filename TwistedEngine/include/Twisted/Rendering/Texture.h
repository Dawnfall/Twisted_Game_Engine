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

	struct TWISTED_API Texture :public TObject //assumes mipmap, rgba 4 channel input , assumes valid texture
	{
		Texture(const std::string& name);
		Texture(const std::string& name, const TextureParams& params);
		void OnDestroy() override;

		bool IsValid()const { return TexID != 0; }

		TextureParams Params;
		int Width = 0;
		int Height = 0;
		int Channels = 0;
		unsigned int TexID = 0;
	};
}

