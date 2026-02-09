#include "EditorRenderer.h"
#include "Twisted/Gameing/Components/CRenderer.h"
#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/SpotLightComponent.h"
#include "Twisted/Gameing/Components/DirectionalLightComponent.h"
#include "Twisted/Gameing/Components/PointLightComponent.h"
#include <vector>
#include "Twisted/Rendering/Data/RenderContext.h"
#include "Twisted/Gameing/World.h"
#include "Utils/GlmUtils.h"

namespace Twisted::Editor
{
	std::vector<CameraData> EditorRenderer::CollectCameraData(World& editorWorld, World& gameWorld)const
	{
		(void)gameWorld;
		std::vector<CameraData> camData;

		auto camera = editorWorld.FindFirstOfType<CameraComponent>();

		CameraData data{};
		data.framebuffer = EditorFrameBuffer;
		data.viewMatrix = camera->GetViewMatrix();
		data.projectionMatrix = camera->GetProjectionMatrix();
		data.clearParams = camera->clearParams;

		camData.emplace_back(data);


		return camData;
	}

	std::vector<ModelData> EditorRenderer::CollectModelData(World& editorWorld, World& gameWorld)const
	{
		(void)editorWorld;

		std::vector<ModelData> modelData;

		auto renderers = gameWorld.GetGroup<RendererComponent, TransformComponent>();
		modelData.reserve(renderers.size());
		for (auto&& [ent, r, t] : renderers.each())
		{
			ModelData data{};
			data.material = r.material;
			data.mesh = r.mesh;

			if (!data.material || !data.mesh || !data.material->Shad)
				continue;

			data.modelMatrix = t.GetWorldModelMatrix();
			modelData.emplace_back(data);
		}

		return modelData;
	}

	LightData EditorRenderer::CollectLightData(World& editorWorld, World& gameWorld)const
	{
		(void)editorWorld;

		LightData lightData;

		auto dirLights = gameWorld.GetGroup<DirectionalLightComponent, TransformComponent>();
		lightData.dirLights.reserve(dirLights.size());
		for (auto&& [ent, l, t] : dirLights.each())
		{
			DirLightData dirLightData{};

			dirLightData.direction = Vec4f(t.GetWorldForward(), 0.0f);
			dirLightData.lightColor = l.color;
			dirLightData.intensity = l.intensity;

			lightData.dirLights.emplace_back(dirLightData);
		}

		auto spotLights = gameWorld.GetGroup<SpotLightComponent, TransformComponent>();
		lightData.spotLights.reserve(spotLights.size());
		for (auto&& [ent, l, t] : spotLights.each())
		{
			SpotLightData spotLightData{};

			spotLightData.direction = Vec4f(t.GetWorldForward(), 0.0f);
			spotLightData.position = Vec4f(t.GetWorldPosition(), 1.0f);
			spotLightData.lightColor = l.color;
			spotLightData.intensity = l.intensity;
			spotLightData.inner = l.innerAngleDeg;
			spotLightData.outer = l.outerAngleDeg;

			lightData.spotLights.emplace_back(spotLightData);
		}

		auto pointLights = gameWorld.GetGroup<PointLightComponent, TransformComponent>();
		lightData.pointLights.reserve(pointLights.size());
		for (auto&& [ent, l, t] : pointLights.each())
		{
			PointLightData pointLightData{};

			pointLightData.direction = Vec4f(t.GetWorldForward(), 0.0f);
			pointLightData.position = Vec4f(t.GetWorldPosition(), 1.0f);
			pointLightData.lightColor = l.color;
			pointLightData.intensity = l.intensity;

			lightData.pointLights.emplace_back(pointLightData);
		}

		return lightData;
	}
}