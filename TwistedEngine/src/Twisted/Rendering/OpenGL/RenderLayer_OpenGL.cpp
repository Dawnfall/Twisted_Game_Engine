
#include "Debug/Logger.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "Twisted/Rendering/Data/ShaderLayouts.h"

#include <glad/glad.h>
#include <string>

#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
#include "Twisted/Rendering/OpenGL/Shader_OpenGL.h"
#include "Twisted/Rendering/OpenGL/Mesh_OpenGL.h"

namespace Twisted
{
	void RenderLayer::Init()
	{
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

	}

	void RenderLayer::Render()
	{
		for (const auto& camData : m_context.camDatas)
		{
			FrameBuffer_GL::Bind(*camData.framebuffer);

			FrameBuffer_GL::ClearBuffer(camData.clearParams);

			for (const auto& modelData : m_context.modelDatas)
			{
				Shader_GL::Bind(*modelData.material->Shader);
				modelData.material->ApplyUniforms();
				Mesh_GL::Bind(*modelData.mesh);

				ShaderMVPBuffer data;
				data.ModelMatrix = modelData.modelMatrix;
				data.ProjectionMatrix = camData.projectionMatrix;
				data.ViewMatrix = camData.viewMatrix;
				Shader_GL::SetBuffer("MVP_uniforms", *modelData.material->Shader,&data);

				size_t primitiveCount = modelData.mesh->IndexCount;
				//Mesh_GL::Render(model.mesh);
				glDrawElements(
					GL_TRIANGLES,                    // primitive type
					static_cast<GLsizei>(modelData.mesh->IndexCount), // number of indices
					GL_UNSIGNED_INT,                  // type of indices
					reinterpret_cast<void*>(0)                         // offset into EBO
				);
				Mesh_GL::UnBind();
				Shader_GL::UnBind();
			}
			FrameBuffer_GL::UnBind();
		}
		Clear();
	}
}

