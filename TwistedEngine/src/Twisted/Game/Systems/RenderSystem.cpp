#include "twistedpch.h"
#include "RenderSystem.h"
#include "Twisted/AppBase.h"
#include "Constants.h"

#include "Twisted/Rendering/RenderingAPI.h"

#include "Twisted/Game/World.h"
#include "Twisted/Game/Components/CRenderer.h"
#include "Twisted/Game/Components/CTransform.h"
#include "Twisted/Game/Components/CCamera.h"


namespace Twisted
{
	void RenderSystem::Update(AppBase* app)
	{
		auto world = app->GetWorld();
		auto cameras = world->GetComponents<CCamera>();
		auto renderers = world->GetComponents<CRenderer, CTransform>();

		for (auto cameraEntity : cameras)
		{
			CCamera& camera = cameras.get<CCamera>(cameraEntity);

			for (auto rendEntity : renderers)
			{
				auto [r, t] = renderers.get<CRenderer, CTransform>(rendEntity);

				r.Material->SetMat4x4f(SHADER_UNIFORM_NAME_MODEL, t.GetWorldModelMatrix());
				r.Material->SetMat4x4f(SHADER_UNIFORM_NAME_PROJECTION, camera.GetProjectionMatrix());
				r.Material->SetMat4x4f(SHADER_UNIFORM_NAME_VIEW, camera.GetViewMatrix());

				RenderAPI::Render(r);
			}
		}
	}
};