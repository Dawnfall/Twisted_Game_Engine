#include "Vulkan/VulkanDescriptors.h"
#include "Vulkan/VulkanDevice.h"
#include "Data/ShaderLayouts.h"
#include "Debug/Logger.h"

#include <stdexcept>
#include <array>
#include <vector>

namespace Twisted::VK
{

void VulkanDescriptors::Init(VulkanDevice& dev)
{
    CreateLayout(dev.Handle);
    CreateUBOs(dev);
    CreatePool(dev.Handle);
    CreateSets(dev.Handle);
}

void VulkanDescriptors::RebuildSets(VkDevice device)
{
    CreateSets(device);
}

void VulkanDescriptors::Shutdown(VkDevice device)
{
    for (int i = 0; i < Render::FramesInFlight; ++i)
    {
        if (!MvpUBOs.empty() && MvpUBOs[i])
        {
            vkUnmapMemory(device, MvpUBOMemory[i]);
            vkDestroyBuffer(device, MvpUBOs[i], nullptr);
            vkFreeMemory(device, MvpUBOMemory[i], nullptr);
        }
        if (!LightsUBOs.empty() && LightsUBOs[i])
        {
            vkUnmapMemory(device, LightsUBOMemory[i]);
            vkDestroyBuffer(device, LightsUBOs[i], nullptr);
            vkFreeMemory(device, LightsUBOMemory[i], nullptr);
        }
    }
    if (GlobalPool   != VK_NULL_HANDLE) { vkDestroyDescriptorPool(device, GlobalPool, nullptr);             GlobalPool   = VK_NULL_HANDLE; }
    if (GlobalLayout != VK_NULL_HANDLE) { vkDestroyDescriptorSetLayout(device, GlobalLayout, nullptr);      GlobalLayout = VK_NULL_HANDLE; }
}

void VulkanDescriptors::CreateLayout(VkDevice device)
{
    std::array<VkDescriptorSetLayoutBinding, 2> bindings{};

    bindings[0].binding         = 0;
    bindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags      = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    bindings[1].binding         = 1;
    bindings[1].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[1].descriptorCount = 1;
    bindings[1].stageFlags      = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo ci{};
    ci.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    ci.bindingCount = static_cast<uint32_t>(bindings.size());
    ci.pBindings    = bindings.data();

    if (vkCreateDescriptorSetLayout(device, &ci, nullptr, &GlobalLayout) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create global descriptor set layout");
}

void VulkanDescriptors::CreateUBOs(VulkanDevice& dev)
{
    VkPhysicalDeviceProperties devProps{};
    vkGetPhysicalDeviceProperties(dev.Physical, &devProps);
    const VkDeviceSize alignment = devProps.limits.minUniformBufferOffsetAlignment;
    const VkDeviceSize mvpBase   = sizeof(ShaderMVPBuffer);
    MvpSlotStride = alignment > 1 ? (mvpBase + alignment - 1) & ~(alignment - 1) : mvpBase;

    const VkDeviceSize mvpSize    = MvpSlotStride * Render::MaxCamerasPerFrame;
    const VkDeviceSize lightsSize = sizeof(ShaderLightsBuffer);

    MvpUBOs.resize(Render::FramesInFlight);
    MvpUBOMemory.resize(Render::FramesInFlight);
    MvpUBOMapped.resize(Render::FramesInFlight);
    LightsUBOs.resize(Render::FramesInFlight);
    LightsUBOMemory.resize(Render::FramesInFlight);
    LightsUBOMapped.resize(Render::FramesInFlight);

    for (int i = 0; i < Render::FramesInFlight; ++i)
    {
        dev.CreateBuffer(mvpSize,
                         VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         MvpUBOs[i], MvpUBOMemory[i]);
        vkMapMemory(dev.Handle, MvpUBOMemory[i], 0, mvpSize, 0, &MvpUBOMapped[i]);

        dev.CreateBuffer(lightsSize,
                         VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         LightsUBOs[i], LightsUBOMemory[i]);
        vkMapMemory(dev.Handle, LightsUBOMemory[i], 0, lightsSize, 0, &LightsUBOMapped[i]);
    }
}

void VulkanDescriptors::CreatePool(VkDevice device)
{
    const uint32_t globalSets = static_cast<uint32_t>(Render::FramesInFlight * Render::MaxCamerasPerFrame);

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = globalSets * 2;
    poolSizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(Render::FramesInFlight) * 64;

    VkDescriptorPoolCreateInfo ci{};
    ci.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    ci.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    ci.pPoolSizes    = poolSizes.data();
    ci.maxSets       = globalSets + static_cast<uint32_t>(Render::FramesInFlight) * 32;
    ci.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    if (vkCreateDescriptorPool(device, &ci, nullptr, &GlobalPool) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create global descriptor pool");
}

void VulkanDescriptors::CreateSets(VkDevice device)
{
    if (GlobalLayout == VK_NULL_HANDLE || GlobalPool == VK_NULL_HANDLE) return;

    const int totalSets = Render::FramesInFlight * Render::MaxCamerasPerFrame;

    if (!GlobalSets.empty())
        vkFreeDescriptorSets(device, GlobalPool,
                             static_cast<uint32_t>(GlobalSets.size()),
                             GlobalSets.data());

    std::vector<VkDescriptorSetLayout> layouts(totalSets, GlobalLayout);
    GlobalSets.resize(totalSets);

    VkDescriptorSetAllocateInfo ai{};
    ai.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    ai.descriptorPool     = GlobalPool;
    ai.descriptorSetCount = static_cast<uint32_t>(totalSets);
    ai.pSetLayouts        = layouts.data();

    if (vkAllocateDescriptorSets(device, &ai, GlobalSets.data()) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to allocate global descriptor sets");

    for (int frame = 0; frame < Render::FramesInFlight; ++frame)
    {
        VkDescriptorBufferInfo lightsInfo{};
        lightsInfo.buffer = LightsUBOs[frame];
        lightsInfo.offset = 0;
        lightsInfo.range  = sizeof(ShaderLightsBuffer);

        for (int slot = 0; slot < Render::MaxCamerasPerFrame; ++slot)
        {
            const int idx = frame * Render::MaxCamerasPerFrame + slot;

            VkDescriptorBufferInfo mvpInfo{};
            mvpInfo.buffer = MvpUBOs[frame];
            mvpInfo.offset = static_cast<VkDeviceSize>(slot) * MvpSlotStride;
            mvpInfo.range  = sizeof(ShaderMVPBuffer);

            std::array<VkWriteDescriptorSet, 2> writes{};
            writes[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writes[0].dstSet          = GlobalSets[idx];
            writes[0].dstBinding      = 0;
            writes[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writes[0].descriptorCount = 1;
            writes[0].pBufferInfo     = &mvpInfo;

            writes[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writes[1].dstSet          = GlobalSets[idx];
            writes[1].dstBinding      = 1;
            writes[1].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writes[1].descriptorCount = 1;
            writes[1].pBufferInfo     = &lightsInfo;

            vkUpdateDescriptorSets(device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
        }
    }
}

} // namespace Twisted::VK
