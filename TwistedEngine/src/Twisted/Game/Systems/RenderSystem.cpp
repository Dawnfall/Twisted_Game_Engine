#include "pch.h"
#include "RenderSystem.h"
#include "Debug/Logger.h"

#include "Twisted/Application.h"
#include "Twisted/Game/Components/CRenderer.h"
#include "Twisted/Game/Components/CTransform.h"
#include "Twisted/Game/Components/CCamera.h"

#include "Twisted/Rendering/OpenGL/Render_OpenGL.h"
#include "Twisted/GameCore.h"
#include "Twisted/Game/Transformations.h"

const std::string shaderUniform_model = "model";
const std::string shaderUniform_view = "view";
const std::string shaderUniform_projection = "projection";

namespace Twisted
{
	void RenderSystem::Update(Application* app)
	{
		auto cameras = app->Game.Ecs.GetComponents<CCamera>();
		auto renderers = app->Game.Ecs.GetComponents<CRenderer, CTransform>();

		for (auto cameraEntity : cameras)
		{
			CCamera& camera = cameras.get<CCamera>(cameraEntity);

			for (auto rendEntity : renderers)
			{
				auto [r, t] = renderers.get<CRenderer, CTransform>(rendEntity);

				r.Material->SetMat4x4f(shaderUniform_model, t.GetWorldModelMatrix(app->Game.Ecs));
				r.Material->SetMat4x4f(shaderUniform_projection, camera.GetProjectionMatrix());
				r.Material->SetMat4x4f(shaderUniform_view, camera.GetViewMatrix(app->Game.Ecs));

				Render_OpenGL::Render(r);
			}
		}
	}
};