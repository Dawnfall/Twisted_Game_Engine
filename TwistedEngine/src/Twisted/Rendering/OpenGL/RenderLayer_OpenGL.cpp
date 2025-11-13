
#include "Debug/Logger.h"
#include "Twisted/Rendering/RenderLayer.h"
#include <glad/glad.h>
#include "Twisted/Constants.h"

#include <string>
namespace Twisted
{
	static GLenum MeshTypeToGL(MeshPrimitiveType type)
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

	void RenderLayer::Render()
	{
		for (const auto& camData : m_context.camDatas)
		{
			camData.framebuffer->Bind();

			// --- Render to framebuffer ---
			glViewport(0, 0, camData.framebuffer->GetSize().x, camData.framebuffer->GetSize().y);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (const auto& modelData : m_context.modelDatas)
			{
				modelData.material->GetShader()->Bind();
				modelData.mesh->Bind();

				size_t primitiveCount = modelData.mesh->GetIndexCount();
				glDrawElements(
					GL_TRIANGLES,                    // primitive type
					static_cast<GLsizei>(modelData.mesh->GetIndexCount()), // number of indices
					GL_UNSIGNED_INT,                  // type of indices
					reinterpret_cast<void*>(0)                         // offset into EBO
				);
				modelData.mesh->UnBind();
				modelData.material->GetShader()->UnBind();

				camData.framebuffer->UnBind();

















			//	modelData.material->ApplyUniforms();

			//	modelData.material->Set<Mat4x4f>(MODEL_MATRIX_NAME, modelData.modelMatrix);
			//	modelData.material->Set<Mat4x4f>(VIEW_MATRIX_NAME, camData.viewMatrix);
			//	modelData.material->Set<Mat4x4f>(PROJ_MATRIX_NAME, camData.projectionMatrix);

			}
			camData.framebuffer->UnBind();
		}
		Clear();
	}

	void RenderLayer::SetClearParams()
	{
		if (m_clearBits)
		{
			if (m_clearParams.doClearColor)
				glClearColor(m_clearParams.clearColor.r, m_clearParams.clearColor.g, m_clearParams.clearColor.b, m_clearParams.clearColor.a);
			if (m_clearParams.doDepthClear)
				glClearDepth(1.0f);
			if (m_clearParams.doClearStencil)
				glClearStencil(0);
			glClear(m_clearBits);
		}
	}

	void RenderLayer::ApplyClearParams()
	{
		// Precompute clear bits
//m_clearBits = 0;
//if (m_params.doClearColor)   m_clearBits |= GL_COLOR_BUFFER_BIT;
//if (m_params.doClearDepth)   m_clearBits |= GL_DEPTH_BUFFER_BIT;
//if (m_params.doClearStencil) m_clearBits |= GL_STENCIL_BUFFER_BIT;

		if (m_clearParams.doDepthTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		if (m_clearParams.doClearStencil)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);

		glDepthMask(m_clearParams.doDepthWrite ? GL_TRUE : GL_FALSE);
	}

}

