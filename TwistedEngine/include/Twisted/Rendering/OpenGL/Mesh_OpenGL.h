#pragma once

#include "AppCore.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Data/MeshData.h"

#include <glad/glad.h>

namespace Twisted::Mesh_GL
{
	[[nodiscard]] GLenum TWISTED_API MeshTypeToGL(MeshPrimitiveType type);

	[[nodiscard]] GLenum TWISTED_API PrimitiveTypeToGL(MeshPrimitiveType primitveType);

	[[nodiscard]] GLenum TWISTED_API DrawTypeToGL(MeshDrawType drawType);

	[[nodiscard]] size_t TWISTED_API CalcNumberPrimitives(MeshPrimitiveType primitiveType, size_t indexCount);

	void TWISTED_API SetData(Mesh& mesh, const PackedMeshData& packedData, MeshDrawType drawType);

	void TWISTED_API ApplyBuffers(const Mesh& mesh, const PackedMeshData& packedData);

	void TWISTED_API Clear(Mesh& mesh);

	void TWISTED_API Bind(const Mesh& mesh);

	void TWISTED_API UnBind();

	void TWISTED_API SetWindingOrder(Mesh& mesh, MeshWindingOrder windOrder);

	void TWISTED_API SetMeshLayout(const PackedMeshData& packedData);

	void TWISTED_API RenderMesh(const Mesh& mesh);
}