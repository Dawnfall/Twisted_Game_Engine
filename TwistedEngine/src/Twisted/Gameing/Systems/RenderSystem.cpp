#include "RenderSystem.h"

//#include "Twisted/Rendering/RenderingAPI.h"
//
//#include "Twisted/Gameing/World.h"
//#include "Twisted/Gameing/Components/CRenderer.h"
//#include "Twisted/Gameing/Components/CTransform.h"
//#include "Twisted/Gameing/Components/CCamera.h"
//
//#include "Twisted/Rendering/Mesh.h"
//#include "Twisted/Rendering/Material.h"
//#include "Twisted/Rendering/RenderContext.h"

namespace Twisted
{
	//void RenderSystem::Update()
	//{
		/*for (auto& entry : m_entries)
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
		}*/
	//}
};