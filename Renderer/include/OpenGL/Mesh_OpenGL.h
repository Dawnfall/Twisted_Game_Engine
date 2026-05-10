#ifdef TWISTED_BACKEND_OPENGL
#pragma once

#include "AppCore.h"
#include "Mesh.h"
#include "Data/MeshData.h"

#include <glad/glad.h>

namespace Twisted
{
	struct MeshBackend
	{
		GLuint Vao = 0;
		GLuint Vbo = 0;
		GLuint Ebo = 0;
	};
}

namespace Twisted::GL
{

	inline void BindMesh(GLuint vao)
	{
		glBindVertexArray(vao);
	}
	inline void UnbindMesh()
	{
		glBindVertexArray(0);
	}

	[[nodiscard]] GLenum TWISTED_API MeshTypeToGL(MeshPrimitiveType type);

	[[nodiscard]] GLenum TWISTED_API PrimitiveTypeToGL(MeshPrimitiveType primitveType);

	[[nodiscard]] GLenum TWISTED_API DrawTypeToGL(MeshDrawType drawType);

	[[nodiscard]] size_t TWISTED_API CalcNumberPrimitives(MeshPrimitiveType primitiveType, size_t indexCount);

	void TWISTED_API SetMeshLayout(const PackedMeshData& packedData);

	void Render(GLsizei indexCount, MeshPrimitiveType primitiveType);

}

#endif