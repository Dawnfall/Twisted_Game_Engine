#pragma once

#include "Twisted/Rendering/OpenGL/Render_OpenGL.h"
#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
#include "Twisted/Rendering/OpenGL/Shader_OpenGL.h"
#include "Twisted/Rendering/OpenGL/Mesh_OpenGL.h"

#include "Twisted/Rendering/Data/ShaderLayouts.h"

namespace Twisted::RenderAPI
{
	inline void Init()
	{
		Render_GL::Init();
	}

	inline void ForwardRender(const RenderContext& context)
	{
		for (const auto& camData : context.camDatas)
		{
			FrameBuffer_GL::Bind(*camData.framebuffer);
			FrameBuffer_GL::ClearBuffer(camData.clearParams);

			for (const auto& modelData : context.modelDatas)
			{
				Shader_GL::Bind(*modelData.material->Shader);
				modelData.material->ApplyUniforms();
				Mesh_GL::Bind(*modelData.mesh);

				ShaderMVPBuffer data;
				data.ModelMatrix = modelData.modelMatrix;
				data.ProjectionMatrix = camData.projectionMatrix;
				data.ViewMatrix = camData.viewMatrix;
				Shader_GL::SetBuffer("MVP_uniforms", *modelData.material->Shader, &data);

				Shader_GL::SetBuffer("Lights", *modelData.material->Shader, &context.lightData);

				//size_t primitiveCount = modelData.mesh->IndexCount;
				Mesh_GL::RenderMesh(*modelData.mesh);

				Mesh_GL::UnBind();
				Shader_GL::UnBind();
			}
			FrameBuffer_GL::UnBind();
		}
	}
}