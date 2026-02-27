#pragma once
#include "AppCore.h"
#include "Twisted/TObject.h"
#include <string>
#include "Utils/GlmUtils.h"
#include "Data/TextureData.h"


namespace Twisted
{
    enum class TextureDimension
    {
        Tex2D,
        Tex3D,
        Cube
    };

    enum class TextureFormat
    {
        RGBA8,
        RGBA16F,
        RGBA32F,
        Depth24Stencil8,
        // extend as needed
    };

    enum TextureUsageFlags : uint32_t
    {
        TextureUsage_Sampled = 1 << 0,
        TextureUsage_Storage = 1 << 1,
        TextureUsage_ColorAttachment = 1 << 2,
        TextureUsage_DepthAttachment = 1 << 3,
        TextureUsage_TransferSrc = 1 << 4,
        TextureUsage_TransferDst = 1 << 5,
    };

    struct TextureInfo
    {
        TextureDimension Dimension = TextureDimension::Tex2D;

        int Width = 1;
        int Height = 1;
        int Depth = 1;

        TextureFormat Format = TextureFormat::RGBA8;

        uint32_t MipLevels = 1;
        uint32_t ArrayLayers = 1;
        uint32_t SampleCount = 1;

        uint32_t UsageFlags = TextureUsage_Sampled;
    };

	struct TextureBackend;
	struct TWISTED_API Texture :public TObject //assumes mipmap, rgba 4 channel input , assumes valid texture
	{
		Texture(const std::string& name);
		~Texture();
		void OnDestroy() override;

		void SetData(const TextureData& data,TextureParams params);
		void Clear();
		void Resize(const Vec2i& newSize);

		const TextureBackend* GetBackend()const { return m_backend; }

        // Engine-visible identity / description
        TextureInfo Info;
        // Optional sampler state (can later be split into a Sampler object)
        TextureParams Params;
        // Version increments when resized / recreated
        uint32_t Version = 0;

	private:

		TextureBackend* m_backend=nullptr;
	};

}

