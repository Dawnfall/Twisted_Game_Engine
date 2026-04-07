#ifndef TWISTED_D3D

#include "Twisted/Rendering/RenderAPI.h"
#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
#include "Twisted/Rendering/OpenGL/Shader_OpenGL.h"
#include "Twisted/Rendering/OpenGL/Mesh_OpenGL.h"
#include "Twisted/Rendering/OpenGL/UniformBuffer_OpenGL.h"
#include "Twisted/Rendering/Data/ShaderLayouts.h"
#include "Twisted/Rendering/Data/RenderContext.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/Shader.h"

#include <glad/glad.h>
#include "Debug/Logger.h"
#include "Utils/GlmUtils.h"

namespace Twisted::Render
{
	static GL::UniformBuffer<ShaderMVPBuffer>    s_mvpBuffer;
	static GL::UniformBuffer<ShaderLightsBuffer> s_lightsBuffer;

	static ShaderLightsBuffer BuildLightsBuffer(const LightData& lightData)
	{
		ShaderLightsBuffer lights{};

		lights.dirLightCount = static_cast<int>(lightData.dirLights.size());
		for (int i = 0; i < lights.dirLightCount; ++i)
			lights.dirLights[i] = lightData.dirLights[i];

		lights.pointLightCount = static_cast<int>(lightData.pointLights.size());
		for (int i = 0; i < lights.pointLightCount; ++i)
			lights.pointLights[i] = lightData.pointLights[i];

		lights.spotLightCount = static_cast<int>(lightData.spotLights.size());
		for (int i = 0; i < lights.spotLightCount; ++i)
			lights.spotLights[i] = lightData.spotLights[i];

		return lights;
	}

	void ForwardRender(const RenderContext& context)
	{
		if (s_mvpBuffer.id == 0) 
			s_mvpBuffer.Create();
		if (s_lightsBuffer.id == 0)
			s_lightsBuffer.Create();

		const ShaderLightsBuffer lights = BuildLightsBuffer(context.lightData);
		if (s_lightsBuffer.id != 0)
			s_lightsBuffer.Upload(lights);

		glDisable(GL_CULL_FACE); //TODO: remove — testing winding order
		EnableDepthTest(true);
		EnableStencilTest(false);

		for (const auto& camData : context.camDatas)
		{
			camData.framebuffer->Bind();
			GL::ClearBuffer(camData.clearParams);

			for (const auto& modelData : context.modelDatas)
			{
				auto shader_gl = modelData.material->GetShader()->GetBackend();
				auto mesh_gl   = modelData.mesh->GetBackend();

				if (shader_gl->UsesBlock(ShaderMVPBuffer::kBindingPoint))
				{
					ShaderMVPBuffer mvp;
					mvp.ModelMatrix      = modelData.modelMatrix;
					mvp.ViewMatrix       = camData.viewMatrix;
					mvp.ProjectionMatrix = camData.projectionMatrix;

					//TWISTED_INFO("ProgramID={} VAO={}", shader_gl->ProgramID, mesh_gl->Vao);
					//TWISTED_INFO("ModelMatrix: {}", ::Utils::toString(modelData.modelMatrix));
					//TWISTED_INFO("ViewMatrix: {}", ::Utils::toString(camData.viewMatrix));
					//TWISTED_INFO("ProjectionMatrix: {}", ::Utils::toString(camData.projectionMatrix));

					s_mvpBuffer.Upload(mvp);
				}


				GL::BindShader(shader_gl->ProgramID);
				modelData.material->ApplyUniforms();
				GL::BindMesh(mesh_gl->Vao);

				GL::Render(static_cast<GLsizei>(modelData.mesh->GetIndexCount()), modelData.mesh->GetPrimitiveType());
				GL::UnbindMesh();
				GL::UnbindShader();

				if (GLenum err = glGetError(); err != GL_NO_ERROR)
					TWISTED_WARN("GL error after draw: {:#x}", err);
			}
			camData.framebuffer->Unbind();
		}
	}

	void EnableDepthTest(bool doTest)
	{
		if (doTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		glDepthMask(doTest ? GL_TRUE : GL_FALSE);
	}

	void EnableStencilTest(bool doTest)
	{
		if (doTest)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
		glStencilMask(doTest ? GL_TRUE : GL_FALSE);
	}
}

#endif
