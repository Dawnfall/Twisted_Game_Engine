#pragma once
#include "AppCore.h"
#include "Application/TObject.h"
#include "Data/MeshData.h"

namespace Twisted
{
	struct MeshBackend;
	struct TWISTED_API Mesh :public TObject
	{
		Mesh(const std::string& name);
		~Mesh();
		void OnDestroy()override;

		void Clear();
		void SetData(const PackedMeshData& packedData, MeshDrawType drawType);

		const MeshBackend* GetBackend() const { return m_backend; }
		size_t GetIndexCount()const { return m_indexCount; }
		MeshPrimitiveType GetPrimitiveType() const { return m_primitiveType; }
	private:

		MeshBackend* m_backend = nullptr;

		MeshWindingOrder m_windOrder = MeshWindingOrder::COUNTER_CLOCKWISE;
		MeshDrawType m_drawType = MeshDrawType::DYNAMIC;
		MeshPrimitiveType m_primitiveType = MeshPrimitiveType::TRIANGLES;
		size_t m_indexCount = 0;
	};
}

