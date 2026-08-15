#pragma once
#include "RenderConstants.h"
#include <vulkan/vulkan.h>

namespace Twisted::VK
{
    struct LineRenderer
    {
        void Init();
        void Shutdown();

        // Draw all pending DebugDraw lines into the active camera pass.
        void Render(VkCommandBuffer cmd, VkDescriptorSet globalDescSet);

    private:
        VkPipeline       m_pipeline       = VK_NULL_HANDLE;
        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

        static constexpr uint32_t k_MaxVertices = 16384;
        VkBuffer       m_vertexBuffers[Render::FramesInFlight] = {};
        VkDeviceMemory m_vertexMemory[Render::FramesInFlight]  = {};
        void*          m_vertexMapped[Render::FramesInFlight]  = {};
    };
}
