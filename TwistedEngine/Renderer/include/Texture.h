#pragma once
#include "AppCore.h"
#include "Application/TObject.h"
#include <string>
#include "Utils/GlmUtils.h"
#include "Data/TextureData.h"
#include <vulkan/vulkan.h>


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
        RGBA8_SRGB,
        RGBA16F,
        RGBA32F,
        Depth24Stencil8,
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

	struct TWISTED_API Texture : public TObject
	{
		Texture(const std::string& name);
		~Texture() override;
		void OnDestroy() override;

		void SetData(const TextureData& data, TextureParams params);
		void Clear();
		void Resize(const Vec2i& newSize);

		bool IsValid() const { return Image != VK_NULL_HANDLE; }

        TextureInfo Info;
        TextureParams Params;
        uint32_t Version = 0;

        VkImage        Image      = VK_NULL_HANDLE;
        VkImageView    ImageView  = VK_NULL_HANDLE;
        VkDeviceMemory Memory     = VK_NULL_HANDLE;
        VkSampler      Sampler    = VK_NULL_HANDLE;
	};

}

