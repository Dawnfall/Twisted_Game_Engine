#include "RenderSystem.h"
#include "Debug/Logger.h"

#include "Dawn/Game/Components/CRenderer.h"
#include "Dawn/Game/Components/CTransform.h"
#include "Dawn/Game/Components/CCamera.h"

#include "Dawn/Rendering/OpenGL/Render_OpenGL.h"
#include "Dawn/GameCore.h"

#include <string>

const std::string shaderUniform_model = "model";
const std::string shaderUniform_view = "view";
const std::string shaderUniform_projection = "projection";

namespace Dawn
{
	void RenderSystem::Update(Application& app)
	{
		auto cameras = app.Game.GetRegistry().view<CCamera>();
		auto renderers = app.Game.GetRegistry().view<CRenderer, CTransform>();

		for (auto cameraEntity : cameras)
		{
			CCamera& camera = cameras.get<CCamera>(cameraEntity);

			for (auto rendEntity : renderers)
			{
				auto [r, t] = renderers.get<CRenderer, CTransform>(rendEntity);

				r.Material->SetMat4x4f(shaderUniform_model, t.GetWorldModelMatrix(app.Game));
				r.Material->SetMat4x4f(shaderUniform_projection, camera.GetProjectionMatrix());
				r.Material->SetMat4x4f(shaderUniform_view, camera.GetViewMatrix(app.Game));

				Render_OpenGL::Render(r);
			}
		}
	}
};