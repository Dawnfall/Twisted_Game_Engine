#ifndef TWISTED_D3D

#include "Twisted/Rendering/OpenGL/Render_OpenGL.h"
#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
#include "Twisted/Rendering/OpenGL/Shader_OpenGL.h"
#include "Twisted/Rendering/OpenGL/Mesh_OpenGL.h"
#include "Twisted/Rendering/Data/ShaderLayouts.h"
#include "Twisted/Rendering/Data/RenderContext.h"
#include "Twisted/Rendering/RenderAPI.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/Shader.h"

#include <glad/glad.h>

namespace Twisted
{

}

namespace Twisted::GL
{
	void Init()
	{
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void ForwardRender(const RenderContext& context)
	{
		for (const auto& camData : context.camDatas)
		{
			auto fb_gl = camData.framebuffer->GetBackend();
			GL::BindFramebuffer(fb_gl->Id);
			GL::ClearBuffer(camData.clearParams);

			for (const auto& modelData : context.modelDatas)
			{
				auto shader_gl = modelData.material->GetShader()->GetBackend();
				auto mesh_gl = modelData.mesh->GetBackend();

				GL::BindShader(shader_gl->ProgramID);
				modelData.material->ApplyUniforms();
				GL::BindMesh(mesh_gl->Vao);

				ShaderMVPBuffer data;
				data.ModelMatrix = modelData.modelMatrix;
				data.ProjectionMatrix = camData.projectionMatrix;
				data.ViewMatrix = camData.viewMatrix;

				//GL::SetUniformBuffer(&data,); //"MVP_uniforms"
				//GL::SetUniformBuffer(&data,); //"Lights"

				GL::Render(static_cast<GLsizei>(modelData.mesh->GetIndexCount()));
				GL::UnbindMesh();
				GL::UnbindShader();
			}
			GL::UnbindFramebuffer();
		}
	}

	void EnableDepthTest(bool doTest)
	{
		if (doTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		glDepthMask(doTest ? GL_TRUE : GL_FALSE); //TODO... maybe separate function
	}

	void EnableStencilTest(bool doTest)
	{
		if (doTest)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
		glStencilMask(doTest ? GL_TRUE : GL_FALSE); //TODO... maybe separate function
	}
}
#endif