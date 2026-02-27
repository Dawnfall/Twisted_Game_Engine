#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/OpenGL/Mesh_OpenGL.h"
#include "Twisted/Rendering/Data/MeshData.h"

#include "Debug/Logger.h"

#include <string>
#include <glad/glad.h>

namespace Twisted
{
	Mesh::Mesh(const std::string& name) :
		TObject(name),
		m_backend(new MeshBackend())
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
		glDeleteVertexArrays(1, &m_backend->Vao);
		glDeleteBuffers(1, &m_backend->Vbo);
		glDeleteBuffers(1, &m_backend->Ebo);

		m_backend->Vbo = m_backend->Ebo = m_backend->Vao = 0;
	}

	void Mesh::SetData(const PackedMeshData& packedData, MeshDrawType drawType)
	{
		Clear();

		if (packedData.VertexBuffer.empty() || packedData.Indices.empty())
			return;

		m_drawType = drawType;
		m_indexCount = packedData.Indices.size();

		glGenVertexArrays(1, &m_backend->Vao);
		glGenBuffers(1, &m_backend->Vbo);
		glGenBuffers(1, &m_backend->Ebo);

		glBindVertexArray(m_backend->Vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_backend->Vbo);
		glBufferData(GL_ARRAY_BUFFER,
			packedData.VertexBuffer.size() * sizeof(float),
			packedData.VertexBuffer.data(),
			GL::DrawTypeToGL(m_drawType)
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_backend->Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			packedData.Indices.size() * sizeof(unsigned int),
			packedData.Indices.data(),
			GL::DrawTypeToGL(m_drawType)
		);

		GL::SetMeshLayout(packedData);
		glBindVertexArray(0);
	}
}

namespace Twisted::GL
{
	GLenum MeshTypeToGL(MeshPrimitiveType type)
	{
		switch (type)
		{
		case MeshPrimitiveType::TRIANGLES:
			return GL_TRIANGLES;
		case MeshPrimitiveType::LINES:
			return GL_LINES;
		case MeshPrimitiveType::POINTS:
			return GL_POINT;
		default:
			TWISTED_ERROR(std::string("Unsupported mesh type"));
			return GL_INVALID_ENUM;
		}
	}

	GLenum PrimitiveTypeToGL(MeshPrimitiveType primitveType)
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

	GLenum DrawTypeToGL(MeshDrawType drawType)
	{
		switch (drawType)
		{
		case MeshDrawType::DYNAMIC: return GL_DYNAMIC_DRAW;
		case MeshDrawType::STREAM: return GL_STREAM_DRAW;
		case MeshDrawType::STATIC: return GL_STATIC_DRAW;
		default: return 0;
		}
	}

	size_t CalcNumberPrimitives(MeshPrimitiveType primitiveType, size_t indexCount)
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

	void Render(GLsizei indexCount)
	{
		glDrawElements(
			GL_TRIANGLES,                 //TODO... should be set on mesh
			static_cast<GLsizei>(indexCount),
			GL_UNSIGNED_INT,
			nullptr
		);
	}

	void SetMeshLayout(const PackedMeshData& packedData)
	{
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


