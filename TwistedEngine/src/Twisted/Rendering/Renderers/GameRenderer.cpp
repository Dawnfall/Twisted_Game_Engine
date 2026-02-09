#include "Twisted/Rendering/Renderers/GameRenderer.h"

#include "Twisted/Gameing/Components/CRenderer.h"
#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/SpotLightComponent.h"
#include "Twisted/Gameing/Components/DirectionalLightComponent.h"
#include "Twisted/Gameing/Components/PointLightComponent.h"

namespace Twisted
{
	std::vector<CameraData> GameRenderer::CollectCameraData(World& world)const
	{
		std::vector<CameraData> camData;

		const auto cameras = world.GetGroup<CameraComponent>();
		camData.reserve(cameras.size());
		for (const auto camEnt : cameras)
		{
			CameraComponent& cam = cameras.get<CameraComponent>(camEnt);
			if (!cam.IsMainCamera())
				continue;

			CameraData data{};
			data.framebuffer = GameFrameBuffer;
			data.viewMatrix = cam.GetViewMatrix();
			data.projectionMatrix = cam.GetProjectionMatrix();
			data.clearParams = cam.clearParams;

			camData.emplace_back(data);
		}

		return camData;
	}

	std::vector<ModelData> GameRenderer::CollectModelData(World& world)const
	{
		std::vector<ModelData> modelData;

		auto renderers = world.GetGroup<RendererComponent, TransformComponent>();
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

	LightData GameRenderer::CollectLightData(World& world)const
	{
		LightData lightData;

		auto dirLights = world.GetGroup<DirectionalLightComponent, TransformComponent>();
		lightData.dirLights.reserve(dirLights.size());
		for (auto&& [ent, l, t] : dirLights.each())
		{
			DirLightData dirLightData;

			dirLightData.direction = Vec4f(t.GetWorldForward(), 0.0f);
			dirLightData.lightColor = l.color;
			dirLightData.intensity = l.intensity;

			lightData.dirLights.emplace_back(dirLightData);
		}

		auto spotLights = world.GetGroup<SpotLightComponent, TransformComponent>();
		lightData.spotLights.reserve(spotLights.size());
		for (auto&& [ent, l, t] : spotLights.each())
		{
			SpotLightData spotLightData;

			spotLightData.direction = Vec4f(t.GetWorldForward(), 0.0f);
			spotLightData.position = Vec4f(t.GetWorldPosition(), 1.0f);
			spotLightData.lightColor = l.color;
			spotLightData.intensity = l.intensity;
			spotLightData.inner = l.innerAngleDeg;
			spotLightData.outer = l.outerAngleDeg;

			lightData.spotLights.emplace_back(spotLightData);
		}

		auto pointLights = world.GetGroup<PointLightComponent, TransformComponent>();
		lightData.pointLights.reserve(pointLights.size());
		for (auto&& [ent, l, t] : pointLights.each())
		{
			PointLightData pointLightData;

			pointLightData.direction = Vec4f(t.GetWorldForward(), 0.0f);
			pointLightData.position = Vec4f(t.GetWorldPosition(), 1.0f);
			pointLightData.lightColor = l.color;
			pointLightData.intensity = l.intensity;

			lightData.pointLights.emplace_back(pointLightData);
		}

		return lightData;
	}
}