#ifndef TWISTED_D3D

#include "Twisted/Rendering/OpenGL/Render_OpenGL.h"
#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
#include "Twisted/Rendering/OpenGL/Shader_OpenGL.h"
#include "Twisted/Rendering/OpenGL/Mesh_OpenGL.h"
#include "Twisted/Rendering/Data/ShaderLayouts.h"
#include "Twisted/Rendering/Data/RenderContext.h"

#include <glad/glad.h>

namespace Twisted::GL
{
	void Init()
	{
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
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

	void ForwardRender(const RenderContext& context)
	{
		//for (const auto& camData : context.camDatas)
		//{
		//	camData.framebuffer->Bind();
		//	GL::ClearBuffer(camData.clearParams);

		//	for (const auto& modelData : context.modelDatas)
		//	{
		//		modelData.material->Shader->Bind();
		//		modelData.material->Shader->ApplyUniforms(*modelData.material);
		//		Mesh_GL::Bind(*modelData.mesh);

		//		ShaderMVPBuffer data;
		//		data.ModelMatrix = modelData.modelMatrix;
		//		data.ProjectionMatrix = camData.projectionMatrix;
		//		data.ViewMatrix = camData.viewMatrix;
		//		GL::SetBuffer("MVP_uniforms", *modelData.material->Shader, &data);

		//		GL::SetBuffer("Lights", *modelData.material->Shader, &context.lightData);

		//		//size_t primitiveCount = modelData.mesh->IndexCount;
		//		Mesh_GL::RenderMesh(*modelData.mesh);

		//		Mesh_GL::UnBind();
		//		GL::UnBind();
		//	}
		//	FrameBuffer_GL::UnBind();
		//}
	}

	void RenderMesh(const Mesh_OpenGL& mesh)
	{
		glDrawElements(
			GL_TRIANGLES,                 //TODO... should be set on mesh
			static_cast<GLsizei>(mesh.IndexCount),
			GL_UNSIGNED_INT,
			nullptr
		);
	}

}

#endif