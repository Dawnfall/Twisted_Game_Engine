#include "Twisted/Rendering/Mesh.h"
#include "Logger.h"
#include <glad/glad.h>

namespace Twisted
{
	static GLenum PrimitiveTypeToGL(MeshPrimitiveType primitveType)
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

	static size_t CalcNumberPrimitives(MeshPrimitiveType primitiveType, size_t indexCount)
	{
		switch (primitiveType)
		{
		case MeshPrimitiveType::POINTS: return indexCount;
		case MeshPrimitiveType::LINES:  return indexCount / 2;
		case MeshPrimitiveType::TRIANGLES: return indexCount / 3;
		}
	}

	static GLenum GetDrawType(MeshDrawType drawType)
	{
		switch (drawType)
		{
		case MeshDrawType::DYNAMIC: return GL_DYNAMIC_DRAW;
		case MeshDrawType::STREAM: return GL_STREAM_DRAW;
		case MeshDrawType::STATIC: return GL_STATIC_DRAW;
		default: return 0;
		}
	}

	void Mesh::Create(const MeshData& meshData,const MeshParams& params)
	{
		m_data = meshData;
		m_params = params;
		m_primitiveCount = CalcNumberPrimitives(meshData.PrimitiveType, meshData.Indices.size());
		m_params = params;

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			m_data.Vertices.size() * sizeof(Vertex),
			m_data.Vertices.data(),
			GetDrawType(m_data.DrawType));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			m_data.Indices.size() * sizeof(unsigned int),
			m_data.Indices.data(),
			GetDrawType(m_data.DrawType));

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

	void Mesh::Clear()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);

		m_vbo = m_ebo = m_vao = 0;
		m_data = MeshData{};
	}

	void Mesh::Bind()
	{
		glBindVertexArray(m_vao);
		glFrontFace(m_params.Order == WindingOrder::CLOCKWISE ? GL_CW : GL_CCW); //this is global state, can be put out

	}

	void Mesh::UnBind()const
	{
		glBindVertexArray(0);
	}
}
