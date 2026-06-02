#pragma once
#include "AppCore.h"
#include "Application/TObject.h"
#include "Data/MeshData.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace Twisted
{
	struct TWISTED_API Mesh : public TObject
	{
		Mesh(const std::string& name);
		~Mesh() override;
		void OnDestroy() override;

		void Clear();
		void SetData(const PackedMeshData& packedData, MeshDrawType drawType);

		bool IsValid() const { return VertexBuffer != VK_NULL_HANDLE; }
		size_t GetIndexCount() const { return m_indexCount; }
		MeshPrimitiveType GetPrimitiveType() const { return m_primitiveType; }

		VkBuffer       VertexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory VertexMemory = VK_NULL_HANDLE;
		VkBuffer       IndexBuffer  = VK_NULL_HANDLE;
		VkDeviceMemory IndexMemory  = VK_NULL_HANDLE;

		uint32_t                     Stride     = 0;
		std::vector<VertexAttribute> Attributes;

	private:
		MeshDrawType      m_drawType      = MeshDrawType::DYNAMIC;
		MeshPrimitiveType m_primitiveType = MeshPrimitiveType::TRIANGLES;
		size_t            m_indexCount    = 0;
	};
}

