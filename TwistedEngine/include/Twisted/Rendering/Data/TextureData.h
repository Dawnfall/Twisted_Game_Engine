#pragma once

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
}