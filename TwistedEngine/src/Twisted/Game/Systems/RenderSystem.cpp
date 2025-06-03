#include "twistedpch.h"
#include "RenderSystem.h"
#include "Constants.h"

#include "Rendering/RenderingAPI.h"

#include "Twisted/Game/World.h"
#include "Twisted/Game/Components/CRenderer.h"
#include "Twisted/Game/Components/CTransform.h"
#include "Twisted/Game/Components/CCamera.h"

#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Rendering/RenderContext.h"

namespace rend = Twisted::Render;

namespace Twisted
{
	void RenderSystem::RegisterContext(rend::RenderContext* context)
	{
		if (std::find(m_contexts.begin(), m_contexts.end(), context) == m_contexts.end())
			m_contexts.emplace_back(context);
	}
	void RenderSystem::UnregisterContext(rend::RenderContext* context)
	{
		auto it = std::find(m_contexts.begin(), m_contexts.end(), context);
		if (it != m_contexts.end())
			m_contexts.erase(it);
	}

	void RenderSystem::Update()
	{
		auto cameras = m_world->GetComponents<CCamera>();
		auto renderers = m_world->GetComponents<CRenderer, CTransform>();

		for (rend::RenderContext* context : m_contexts)
		{
			for (auto cameraEntity : cameras)
			{
				CCamera& camera = cameras.get<CCamera>(cameraEntity);
				context->Bind();
				for (auto rendEntity : renderers)
				{
					auto [r, t] = renderers.get<CRenderer, CTransform>(rendEntity);

					r.Material->SetMat4x4f(SHADER_UNIFORM_NAME_MODEL, t.GetWorldModelMatrix());
					r.Material->SetMat4x4f(SHADER_UNIFORM_NAME_PROJECTION, camera.GetProjectionMatrix());
					r.Material->SetMat4x4f(SHADER_UNIFORM_NAME_VIEW, camera.GetViewMatrix());

					r.Material->GetShader()->Bind();
					r.Material->ApplyUniforms();
					r.Mesh->Bind();

					r.Mesh->Render();

					r.Material->GetShader()->UnBind();
					r.Mesh->UnBind();
				}
				context->UnBind();
			}
		}
	}
};