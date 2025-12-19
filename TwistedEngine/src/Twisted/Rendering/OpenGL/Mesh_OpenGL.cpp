#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/OpenGL/Mesh_OpenGL.h"

#include "Debug/Logger.h"

#include <string>
#include <glad/glad.h>

namespace Twisted
{
	Mesh::Mesh(const std::string& name) :
		TObject(name)
	{

	}

	void Mesh::OnDestroy()
	{
		Mesh_GL::Clear(*this);
	}
}

namespace Twisted::Mesh_GL
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

	void SetData(Mesh& mesh, const PackedMeshData& packedData, MeshDrawType drawType)
	{
		Clear(mesh);

		if (packedData.VertexBuffer.empty() || packedData.Indices.empty())
			return;

		mesh.DrawType = drawType;
		mesh.IndexCount = packedData.Indices.size();

		glGenVertexArrays(1, &mesh.Vao);
		glGenBuffers(1, &mesh.Vbo);
		glGenBuffers(1, &mesh.Ebo);

		glBindVertexArray(mesh.Vao);

		ApplyBuffers(mesh, packedData);
		SetMeshLayout(packedData);

		glBindVertexArray(0);
	}

	void ApplyBuffers(const Mesh& mesh, const PackedMeshData& packedData)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh.Vbo);
		glBufferData(GL_ARRAY_BUFFER,
			packedData.VertexBuffer.size() * sizeof(float),
			packedData.VertexBuffer.data(),
			DrawTypeToGL(mesh.DrawType)
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			packedData.Indices.size() * sizeof(unsigned int),
			packedData.Indices.data(),
			DrawTypeToGL(mesh.DrawType)
		);
	}

	void Clear(Mesh& mesh)
	{
		glDeleteVertexArrays(1, &mesh.Vao);
		glDeleteBuffers(1, &mesh.Vbo);
		glDeleteBuffers(1, &mesh.Ebo);

		mesh.Vbo = mesh.Ebo = mesh.Vao = 0;
	}

	void Bind(const Mesh& mesh)
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(mesh.WindOrder == MeshWindingOrder::CLOCKWISE ? GL_CW : GL_CCW); //this is global state, can be put out
		glCullFace(GL_BACK);
		glBindVertexArray(mesh.Vao);
	}

	void UnBind()
	{
		glBindVertexArray(0);
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

	void RenderMesh(const Mesh& mesh)
	{
		glDrawElements(
			GL_TRIANGLES,                 //TODO... should be set on mesh
			static_cast<GLsizei>(mesh.IndexCount),
			GL_UNSIGNED_INT,                  
			nullptr
		);
	}
}


