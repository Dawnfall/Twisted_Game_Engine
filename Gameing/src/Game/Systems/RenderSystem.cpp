#include "RenderSystem.h"
#include "Constants.h"

#include "Rendering/RenderingAPI.h"

#include "Game/World.h"
#include "Game/Components/CRenderer.h"
#include "Game/Components/CTransform.h"
#include "Game/Components/CCamera.h"

#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Rendering/RenderContext.h"

namespace rend = Twisted::Render;

namespace Twisted
{
	void RenderSystem::Update()
	{
		for (auto& entry : m_entries)
		{
			const auto& camera = m_world->GetComponent<CCamera>(entry.Camera);
			auto renderers = m_world->GetComponents<CRenderer, CTransform>();

			entry.Context->Bind();

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
			entry.Context->UnBind();
		}
	}
};