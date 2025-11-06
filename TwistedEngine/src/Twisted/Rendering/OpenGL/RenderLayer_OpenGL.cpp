
#include "Debug/Logger.h"
#include "Twisted/Rendering/RenderLayer.h"
#include <glad/glad.h>
#include <string>
namespace Twisted
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

	void RenderLayer::Update()
	{
		Render();
		Clear();
	}

	void RenderLayer::Render()
	{
		for (auto& entry : m_entries)
		{
			entry.material->GetShader()->Bind();
			entry.mesh->Bind();

			entry.material->ApplyUniforms();
			glDrawElements(MeshTypeToGL(entry.mesh->GetMeshData().PrimitiveType), static_cast<GLsizei>(entry.mesh->GetPrimitiveCount()), GL_UNSIGNED_INT, nullptr);
			
			switch (entry.mesh->GetMeshData().PrimitiveType)
			{
			case MeshPrimitiveType::TRIANGLES:
				break;
			}

			entry.material->GetShader()->UnBind();
			entry.mesh->UnBind();
		}
	}


}