#include "Twisted/Rendering/Mesh.h"
#include "Debug/Logger.h"
#include <glad/glad.h>
#include <string>

namespace Twisted
{
	[[nodiscard]] static GLenum PrimitiveTypeToGL(MeshPrimitiveType primitveType)
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

	[[nodiscard]] static GLenum DrawTypeToGL(MeshDrawType drawType)
	{
		switch (drawType)
		{
		case MeshDrawType::DYNAMIC: return GL_DYNAMIC_DRAW;
		case MeshDrawType::STREAM: return GL_STREAM_DRAW;
		case MeshDrawType::STATIC: return GL_STATIC_DRAW;
		default: return 0;
		}
	}

	[[nodiscard]] static size_t CalcNumberPrimitives(MeshPrimitiveType primitiveType, size_t indexCount)
	{
		switch (primitiveType)
		{
		case MeshPrimitiveType::POINTS: return indexCount;
		case MeshPrimitiveType::LINES:  return indexCount >= 2 ? indexCount / 2 : 0;
		case MeshPrimitiveType::TRIANGLES: return indexCount >= 3 ? indexCount / 3 : 0;
		default:
			//TWISTED_WARN(std::string("Unsupported mesh type") + std::to_string(primitiveType));
			return 0;
		}
	}


	void Mesh::SetData(const PackedMeshData& packedData, MeshDrawType drawType)
	{
		Clear();

		if (packedData.VertexBuffer.empty() || packedData.Indices.empty())
			return;

		m_drawType = drawType;
		m_indexCount = packedData.Indices.size();

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);

		ApplyBuffers(packedData);
		SetMeshLayout(packedData);

		glBindVertexArray(0);
	}

	void Mesh::ApplyBuffers(const PackedMeshData& packedData)const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			packedData.VertexBuffer.size() * sizeof(float),
			packedData.VertexBuffer.data(),
			DrawTypeToGL(m_drawType)
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			packedData.Indices.size() * sizeof(unsigned int),
			packedData.Indices.data(),
			DrawTypeToGL(m_drawType));
	}

	void Mesh::Clear()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);

		m_vbo = m_ebo = m_vao = 0;
	}

	void Mesh::Bind()const
	{
		glFrontFace(m_windOrder == MeshWindingOrder::CLOCKWISE ? GL_CW : GL_CCW); //this is global state, can be put out
		glBindVertexArray(m_vao);
	}

	void Mesh::UnBind()const
	{
		glBindVertexArray(0);
	}

	void Mesh::SetMeshLayout(const PackedMeshData& packedData)
	{
		// Set layout
		for (unsigned int i = 0; i < packedData.Layout.size(); ++i)
		{
			const auto& attrib = packedData.Layout[i];
			glEnableVertexAttribArray(attrib.Location);
			glVertexAttribPointer(
				attrib.Location,
				static_cast<GLint>(attrib.Count),
				GL_FLOAT,
				GL_FALSE,
				static_cast<GLsizei>(packedData.Stride * sizeof(float)),
				(void*)(attrib.Offset * sizeof(float))
			);
		}
	}
}


