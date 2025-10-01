#include "Twisted/Rendering/Mesh.h"
#include "Logger.h"
#include <glad/glad.h>

namespace Twisted
{
	static int PrimitiveTypeToGL(MeshPrimitiveType primitveType)
	{
		switch (primitveType)
		{
		case Twisted::MeshPrimitiveType::POINTS:
			return GL_POINTS;
		case Twisted::MeshPrimitiveType::LINES:
			return GL_LINES;
		case Twisted::MeshPrimitiveType::TRIANGLES:
			return GL_TRIANGLES;
		default:
			TWISTED_ERROR("Unsupported primitive type");
			return GL_POINTS;
		}
	}

	Mesh::Mesh(ObjectID id, const MeshData& meshData) :
		BaseObject(id),
		m_primitiveType(meshData.PrimitiveType),
		m_primitiveCount(meshData.Indices.size())
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, meshData.Vertices.size() * sizeof(Vertex), &meshData.Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.Indices.size() * sizeof(unsigned int), &meshData.Indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
	}

	void Mesh::Bind()const
	{
		glBindVertexArray(m_vao);
	}

	void Mesh::UnBind()const
	{
		glBindVertexArray(0);
	}

	void Mesh::Render()const
	{
		glDrawElements(PrimitiveTypeToGL(m_primitiveType), (GLsizei)m_primitiveCount, GL_UNSIGNED_INT, 0);
	}
}
