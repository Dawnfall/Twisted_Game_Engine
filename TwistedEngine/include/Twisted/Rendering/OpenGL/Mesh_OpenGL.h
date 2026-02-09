#pragma once

#include "AppCore.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Data/MeshData.h"

#include <glad/glad.h>

namespace Twisted::GL
{
	class Mesh_OpenGL:public TObject
	{
	public:

		void ApplyBuffers(const PackedMeshData& packedData);
		void Clear();
		void Bind()const;
		void UnBind()const;

		MeshWindingOrder WindOrder = MeshWindingOrder::CLOCKWISE;
		MeshDrawType DrawType = MeshDrawType::STATIC;

		unsigned int Vao = 0;
		unsigned int Vbo = 0;
		unsigned int Ebo = 0;
		size_t IndexCount = 0;

	private:
	};

	[[nodiscard]] GLenum TWISTED_API MeshTypeToGL(MeshPrimitiveType type);

	[[nodiscard]] GLenum TWISTED_API PrimitiveTypeToGL(MeshPrimitiveType primitveType);

	[[nodiscard]] GLenum TWISTED_API DrawTypeToGL(MeshDrawType drawType);

	[[nodiscard]] size_t TWISTED_API CalcNumberPrimitives(MeshPrimitiveType primitiveType, size_t indexCount);

	void TWISTED_API SetMeshLayout(const PackedMeshData& packedData);


}