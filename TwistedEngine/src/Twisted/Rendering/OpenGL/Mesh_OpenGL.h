#pragma once

#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Data/MeshData.h"

#include <glad/glad.h>

namespace Twisted::Mesh_GL
{
	[[nodiscard]] GLenum MeshTypeToGL(MeshPrimitiveType type);

	[[nodiscard]] GLenum PrimitiveTypeToGL(MeshPrimitiveType primitveType);

	[[nodiscard]] GLenum DrawTypeToGL(MeshDrawType drawType);

	[[nodiscard]] size_t CalcNumberPrimitives(MeshPrimitiveType primitiveType, size_t indexCount);

	void SetData(Mesh& mesh, const PackedMeshData& packedData, MeshDrawType drawType);

	void ApplyBuffers(const Mesh& mesh, const PackedMeshData& packedData);

	void Clear(Mesh& mesh);

	void Bind(const Mesh& mesh);

	void UnBind();

	void SetWindingOrder(Mesh& mesh, MeshWindingOrder windOrder);

	void SetMeshLayout(const PackedMeshData& packedData);
}