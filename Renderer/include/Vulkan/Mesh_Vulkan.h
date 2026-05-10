#ifdef TWISTED_BACKEND_VULKAN
#pragma once

#include "Data/MeshData.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace Twisted
{
    struct MeshBackend
    {
        VkBuffer       VertexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory VertexMemory = VK_NULL_HANDLE;
        VkBuffer       IndexBuffer  = VK_NULL_HANDLE;
        VkDeviceMemory IndexMemory  = VK_NULL_HANDLE;

        // Runtime vertex layout (from PackedMeshData) — used to build VkVertexInput at draw time
        uint32_t                               Stride = 0;
        std::vector<VertexAttribute>           Attributes;

        bool IsValid() const { return VertexBuffer != VK_NULL_HANDLE; }
    };
}

#endif
