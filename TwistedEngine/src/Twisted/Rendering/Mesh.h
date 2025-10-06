#pragma once
#include "AppCore.h"
#include "Twisted/RegisterLayer/TObject.h"
#include "Twisted/Data/Vertex.h"
#include <filesystem>

namespace Twisted
{
	enum class MeshPrimitiveType
	{
		POINTS,
		LINES,
		TRIANGLES,
		//TODO maybe: GL_LINE_STRIP , GL_LINE_LOOP , GL_TRIANGLE_STRIP , GL_TRIANGLE_FAN
	};

	enum class MeshDrawType
	{
		DYNAMIC,
		STREAM,
		STATIC
	};

	enum class WindingOrder
	{
		CLOCKWISE,
		COUNTER_CLOCKWISE
	};

	struct MeshParams
	{
		WindingOrder Order = WindingOrder::CLOCKWISE;
	};

	struct TWISTED_API MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
		MeshPrimitiveType PrimitiveType = MeshPrimitiveType::TRIANGLES;
		MeshDrawType DrawType = MeshDrawType::DYNAMIC;
	};

	class TWISTED_API Mesh :public TObject // assumes normals are provided  , TODO: type can be smaller for smaller meshes,instancing
	{
	public:
		Mesh() = default;
		Mesh(const MeshData& meshData, const MeshParams& params) :TObject()
		{
			Create(meshData, params);
		}
		~Mesh()
		{
			Clear();
		}

		void Create(const MeshData& meshData, const MeshParams& params);
		void Clear();

		void Bind();
		void UnBind()const;

		bool IsValid()const { return m_vao != 0; }

		void SetDrawType(MeshDrawType drawType)
		{
			if (m_data.DrawType == drawType)
				return;

			m_data.DrawType = drawType;

			Clear();
			Create(m_data, m_params);
		}
		void SetWindingOrder(WindingOrder windOrder)
		{
			if (m_params.Order == windOrder)
				return;

			m_params.Order = windOrder;
		}

		unsigned int GetVAO()const { return m_vao; }
		unsigned int GetVBO()const { return m_vbo; }
		unsigned int GetEBO()const { return m_ebo; }
		size_t GetPrimitiveCount()const { return m_primitiveCount; }
		const MeshData& GetMeshData()const { return m_data; }
		const MeshParams& GetParams()const { return m_params; }

	private:
		unsigned int m_vao = 0;
		unsigned int m_vbo = 0;
		unsigned int m_ebo = 0;
		MeshData m_data;
		MeshParams m_params;
		unsigned int m_primitiveCount = 0;
	};
}