#pragma once
#include "AppCore.h"
#include "Twisted/Rendering/Data/MeshData.h"
#include "Twisted/TObject.h"
#include "Data/Vertex.h"
#include <filesystem>

namespace Twisted
{
	enum class MeshDrawType
	{
		DYNAMIC,
		STREAM,
		STATIC
	};

	enum class MeshWindingOrder
	{
		CLOCKWISE,
		COUNTER_CLOCKWISE
	};

	class TWISTED_API Mesh :public TObject
	{
	public:
		Mesh(const std::string& name):
			TObject(name)
		{ }

		unsigned int GetVAO()const { return m_vao; }
		unsigned int GetVBO()const { return m_vbo; }
		unsigned int GetEBO()const { return m_ebo; }
		size_t GetIndexCount()const { return m_indexCount; }
		MeshDrawType GetDrawType()const { return m_drawType; }
		MeshWindingOrder GetWindOrder()const { return m_windOrder; }

		void SetData(const PackedMeshData& packedData,MeshDrawType drawType);
		void Clear();

		void Bind()const;
		void UnBind()const;

		void SetWindingOrder(MeshWindingOrder windOrder)
		{
			if (m_windOrder == windOrder)
				return;

			m_windOrder = windOrder;
		}

		void OnDestroy()override
		{
			Clear();
		}

	private:
		void ApplyBuffers(const PackedMeshData& packedData)const;
		void SetMeshLayout(const PackedMeshData& packedData);

		unsigned int m_vao = 0;
		unsigned int m_vbo = 0;
		unsigned int m_ebo = 0;
		size_t m_indexCount = 0;

		MeshWindingOrder m_windOrder = MeshWindingOrder::CLOCKWISE;
		MeshDrawType m_drawType = MeshDrawType::STATIC;
	};
}

