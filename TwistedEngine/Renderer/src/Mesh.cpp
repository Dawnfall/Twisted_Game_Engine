#include "Mesh.h"
#include "VulkanContext.h"
#include "Data/MeshData.h"
#include "Debug/Logger.h"

#include <cstring>
#include <stdexcept>

namespace Twisted
{
    Mesh::Mesh(const std::string& name)
        : TObject(name)
    {
    }

    Mesh::~Mesh() = default;

    void Mesh::OnDestroy()
    {
        Clear();
    }

    void Mesh::Clear()
    {
        auto& ctx = VK::VulkanContext::Get();
        if (ctx.Device == VK_NULL_HANDLE) return;

        if (VertexBuffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(ctx.Device, VertexBuffer, nullptr);
            vkFreeMemory(ctx.Device, VertexMemory, nullptr);
        }
        if (IndexBuffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(ctx.Device, IndexBuffer, nullptr);
            vkFreeMemory(ctx.Device, IndexMemory, nullptr);
        }

        VertexBuffer = VK_NULL_HANDLE;
        VertexMemory = VK_NULL_HANDLE;
        IndexBuffer  = VK_NULL_HANDLE;
        IndexMemory  = VK_NULL_HANDLE;
        Stride       = 0;
        Attributes.clear();
    }

    void Mesh::SetData(const PackedMeshData& packedData, MeshDrawType drawType)
    {
        Clear();

        if (packedData.VertexBuffer.empty() || packedData.Indices.empty())
            return;

        m_drawType      = drawType;
        m_primitiveType = packedData.PrimitiveType;
        m_indexCount    = packedData.Indices.size();

        Stride     = static_cast<uint32_t>(packedData.Stride * sizeof(float));
        Attributes = packedData.Layout;

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
                         VertexBuffer, VertexMemory);

        {
            VkCommandBuffer cmd = ctx.BeginSingleTimeCommands();
            VkBufferCopy    copy{ 0, 0, vertSize };
            vkCmdCopyBuffer(cmd, stagingBuf, VertexBuffer, 1, &copy);
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
                         IndexBuffer, IndexMemory);

        {
            VkCommandBuffer cmd = ctx.BeginSingleTimeCommands();
            VkBufferCopy    copy{ 0, 0, idxSize };
            vkCmdCopyBuffer(cmd, stagingBuf, IndexBuffer, 1, &copy);
            ctx.EndSingleTimeCommands(cmd);
        }

        vkDestroyBuffer(ctx.Device, stagingBuf, nullptr);
        vkFreeMemory(ctx.Device, stagingMem, nullptr);
    }
}
