#ifdef TWISTED_BACKEND_VULKAN

#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Vulkan/Mesh_Vulkan.h"
#include "Twisted/Rendering/Vulkan/VulkanContext.h"
#include "Twisted/Rendering/Data/MeshData.h"
#include "Debug/Logger.h"

#include <cstring>
#include <stdexcept>

namespace Twisted
{
    Mesh::Mesh(const std::string& name)
        : TObject(name), m_backend(new MeshBackend())
    {
    }

    Mesh::~Mesh()
    {
        delete m_backend;
    }

    void Mesh::OnDestroy()
    {
        Clear();
    }

    void Mesh::Clear()
    {
        auto& ctx = VK::VulkanContext::Get();
        if (ctx.Device == VK_NULL_HANDLE) return;

        if (m_backend->VertexBuffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(ctx.Device, m_backend->VertexBuffer, nullptr);
            vkFreeMemory(ctx.Device, m_backend->VertexMemory, nullptr);
        }
        if (m_backend->IndexBuffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(ctx.Device, m_backend->IndexBuffer, nullptr);
            vkFreeMemory(ctx.Device, m_backend->IndexMemory, nullptr);
        }
        *m_backend = MeshBackend{};
    }

    void Mesh::SetData(const PackedMeshData& packedData, MeshDrawType drawType)
    {
        Clear();

        if (packedData.VertexBuffer.empty() || packedData.Indices.empty())
            return;

        m_drawType      = drawType;
        m_primitiveType = packedData.PrimitiveType;
        m_indexCount    = packedData.Indices.size();

        m_backend->Stride     = static_cast<uint32_t>(packedData.Stride * sizeof(float));
        m_backend->Attributes = packedData.Layout;

        auto& ctx = VK::VulkanContext::Get();

        // --- Vertex buffer ---
        const VkDeviceSize vertSize = packedData.VertexBuffer.size() * sizeof(float);

        VkBuffer       stagingBuf;
        VkDeviceMemory stagingMem;
        ctx.CreateBuffer(vertSize,
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         stagingBuf, stagingMem);

        void* data = nullptr;
        vkMapMemory(ctx.Device, stagingMem, 0, vertSize, 0, &data);
        memcpy(data, packedData.VertexBuffer.data(), vertSize);
        vkUnmapMemory(ctx.Device, stagingMem);

        ctx.CreateBuffer(vertSize,
                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                         m_backend->VertexBuffer, m_backend->VertexMemory);

        {
            VkCommandBuffer cmd = ctx.BeginSingleTimeCommands();
            VkBufferCopy    copy{ 0, 0, vertSize };
            vkCmdCopyBuffer(cmd, stagingBuf, m_backend->VertexBuffer, 1, &copy);
            ctx.EndSingleTimeCommands(cmd);
        }

        vkDestroyBuffer(ctx.Device, stagingBuf, nullptr);
        vkFreeMemory(ctx.Device, stagingMem, nullptr);

        // --- Index buffer ---
        const VkDeviceSize idxSize = packedData.Indices.size() * sizeof(uint32_t);

        ctx.CreateBuffer(idxSize,
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         stagingBuf, stagingMem);

        vkMapMemory(ctx.Device, stagingMem, 0, idxSize, 0, &data);
        memcpy(data, packedData.Indices.data(), idxSize);
        vkUnmapMemory(ctx.Device, stagingMem);

        ctx.CreateBuffer(idxSize,
                         VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                         m_backend->IndexBuffer, m_backend->IndexMemory);

        {
            VkCommandBuffer cmd = ctx.BeginSingleTimeCommands();
            VkBufferCopy    copy{ 0, 0, idxSize };
            vkCmdCopyBuffer(cmd, stagingBuf, m_backend->IndexBuffer, 1, &copy);
            ctx.EndSingleTimeCommands(cmd);
        }

        vkDestroyBuffer(ctx.Device, stagingBuf, nullptr);
        vkFreeMemory(ctx.Device, stagingMem, nullptr);
    }
}

#endif
