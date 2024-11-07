#include "twistedpch.h"
#include "RenderSystem.h"
#include "Debug/Logger.h"

#include "Twisted/AppBase.h"
#include "Twisted/Game/Components/CRenderer.h"
#include "Twisted/Game/Components/CTransform.h"
#include "Twisted/Game/Components/CCamera.h"

#include "Twisted/Rendering/RenderingAPI.h"
#include "Twisted/Game/Transformations.h"

const std::string shaderUniform_model = "model";
const std::string shaderUniform_view = "view";
const std::string shaderUniform_projection = "projection";

namespace Twisted
{
	void RenderSystem::Update(AppBase* app)
	{
		auto& ecs = app->GetWorld()->GetEcs();
		auto cameras = ecs.GetComponents<CCamera>();
		auto renderers = ecs.GetComponents<CRenderer, CTransform>();

		for (auto cameraEntity : cameras)
		{
			CCamera& camera = cameras.get<CCamera>(cameraEntity);

			for (auto rendEntity : renderers)
			{
				auto [r, t] = renderers.get<CRenderer, CTransform>(rendEntity);

				r.Material->SetMat4x4f(shaderUniform_model, t.GetWorldModelMatrix(ecs));
				r.Material->SetMat4x4f(shaderUniform_projection, camera.GetProjectionMatrix());
				r.Material->SetMat4x4f(shaderUniform_view, camera.GetViewMatrix(ecs));

				RenderAPI::Render(r);
			}
		}
	}
};