#pragma once

#include "RenderConstants.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

namespace Twisted::VK
{
    struct VulkanDevice;

    struct VulkanDescriptors
    {
        VkDescriptorSetLayout        GlobalLayout = VK_NULL_HANDLE;
        VkDescriptorPool             GlobalPool   = VK_NULL_HANDLE;
        std::vector<VkDescriptorSet> GlobalSets;

        std::vector<VkBuffer>       MvpUBOs,       LightsUBOs;
        std::vector<VkDeviceMemory> MvpUBOMemory,  LightsUBOMemory;
        std::vector<void*>          MvpUBOMapped,  LightsUBOMapped;
        VkDeviceSize                MvpSlotStride = 0;

        void Init(VulkanDevice& dev);
        void RebuildSets(VkDevice device);
        void Shutdown(VkDevice device);

    private:
        void CreateLayout(VkDevice device);
        void CreateUBOs(VulkanDevice& dev);
        void CreatePool(VkDevice device);
        void CreateSets(VkDevice device);
    };
}
