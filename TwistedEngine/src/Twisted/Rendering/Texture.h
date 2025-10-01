#pragma once
#include "AppCore.h"
#include "Twisted/RegisterLayer/BaseObject.h"
#include "Twisted/RegisterLayer/Serialization/Serializer.h"
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

	struct TextureData
	{
		unsigned char* Data;
		int Width;
		int Height;
		int Channels;
	};

	class TWISTED_API Texture :public BaseObject //assumes mipmap, rgba 4 channel input , assumes valid texture
	{
	public:
		Texture(ObjectID id);
		Texture(ObjectID id, const TextureData& texData);
		~Texture();

		void Init(const TextureData& data);

		unsigned int GetTexID() const { return m_texID; }

		int GetWidth()const { return m_width; }
		int GetHeight()const { return m_height; }
		int GetChannelsCount()const { return m_channels; }

		TextureWrap GetWrapType()const { return m_wrap; }
		TextureMagFilter GetMagFilter()const { return m_magFilter; }
		TextureMinFilter GetMinFilter()const { return m_minFilter; }

		void SetWrapType(TextureWrap wrapType);
		void SetMagFilter(TextureMagFilter magFilter);
		void SetMinFilter(TextureMinFilter minFilter);

	private:

		TextureWrap m_wrap = TextureWrap::REPEAT;
		TextureMinFilter m_minFilter = TextureMinFilter::NEAREST;
		TextureMagFilter m_magFilter = TextureMagFilter::NEAREST;

		int m_width;
		int m_height;
		int m_channels;
		unsigned int m_texID;
	};

	//template<>
	//void Serialize<Texture>(const Texture& obj, BinSerializer& buffer)
	//{
	//	buffer.Write<int>(obj.GetWidth());
	//	buffer.Write<int>(obj.GetHeight());
	//	buffer.Write<TextureWrap>(obj.GetWrapType());
	//	buffer.Write<TextureMinFilter>(obj.GetMinFilter());
	//	buffer.Write<TextureMagFilter>(obj.GetMagFilter());
	//}
}