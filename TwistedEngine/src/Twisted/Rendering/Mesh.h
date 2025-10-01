#pragma once
#include "AppCore.h"
#include "Twisted/Rendering/MeshPrimitiveType.h"
#include "Twisted/RegisterLayer/BaseObject.h"
#include "Twisted/Data/Vertex.h"
#include <filesystem>

namespace Twisted
{
	struct TWISTED_API MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
		MeshPrimitiveType PrimitiveType = MeshPrimitiveType::TRIANGLES;
	};

	class TWISTED_API Mesh :public BaseObject // assumes normals are provided 
	{
	public:
		Mesh(ObjectID id,const MeshData& meshData);
		~Mesh();

		void Bind()const;
		void UnBind()const;

		void Render()const;

		unsigned int GetVAO()const { return m_vao; }
		unsigned int GetVBO()const { return m_vbo; }
		unsigned int GetEBO()const { return m_ebo; }
		size_t GetPrimitiveCount()const { return m_primitiveCount; }

	private:
		unsigned int m_vao = 0;
		unsigned int m_vbo = 0;
		unsigned int m_ebo = 0;
		size_t m_primitiveCount = 0;
		MeshPrimitiveType m_primitiveType;
	};
}