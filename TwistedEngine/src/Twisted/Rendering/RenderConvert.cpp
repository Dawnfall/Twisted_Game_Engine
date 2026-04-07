#include "Twisted/Rendering/RenderConvert.h"
#include "Twisted/Gameing/World.h"
#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/Components/CRenderer.h"
#include "Twisted/Gameing/Components/DirectionalLightComponent.h"
#include "Twisted/Gameing/Components/PointLightComponent.h"
#include "Twisted/Gameing/Components/SpotLightComponent.h"

namespace Twisted
{
	std::vector<CameraData> CollectCameraData(World& world)
	{
		std::vector<CameraData> camData;

		auto cameras = world.GetView<CameraComponent>();
		for (auto&& [camEnt, cam] : cameras.each())
		{
			if (!cam.IsMainCamera())
				continue;

			CameraData data{};
			data.framebuffer = cam.Fb.get();
			data.viewMatrix = cam.GetViewMatrix();
			data.projectionMatrix = cam.GetProjectionMatrix();
			data.clearParams = cam.clearParams;

			camData.emplace_back(data);
		}

		return camData;
	}

	std::vector<ModelData> CollectModelData(World& world)
	{
		std::vector<ModelData> modelData;

		auto renderers = world.GetGroup<RendererComponent, TransformComponent>();
		modelData.reserve(renderers.size());
		for (auto&& [ent, r, t] : renderers.each())
		{
			ModelData data{};
			data.material = r.material;
			data.mesh = r.mesh;

			if (!data.material || !data.mesh || !data.material->GetShader())
				continue;

			data.modelMatrix = t.GetWorldModelMatrix();
			modelData.emplace_back(data);
		}

		return modelData;
	}

	LightData CollectLightData(World& world)
	{
		LightData lightData;

		auto dirLights = world.GetGroup<DirectionalLightComponent, TransformComponent>();
		lightData.dirLights.reserve(dirLights.size());
		for (auto&& [ent, l, t] : dirLights.each())
		{
			GPUDirLight dirLightData{};
			dirLightData.direction = Vec4f(t.GetWorldForward(), 0.0f);
			dirLightData.lightColor = l.color;
			dirLightData.intensity = l.intensity;
			lightData.dirLights.emplace_back(dirLightData);
		}

		auto spotLights = world.GetGroup<SpotLightComponent, TransformComponent>();
		lightData.spotLights.reserve(spotLights.size());
		for (auto&& [ent, l, t] : spotLights.each())
		{
			GPUSpotLight spotLightData{};
			spotLightData.direction = Vec4f(t.GetWorldForward(), 0.0f);
			spotLightData.position = Vec4f(t.GetWorldPosition(), 1.0f);
			spotLightData.lightColor = l.color;
			spotLightData.intensity = l.intensity;
			spotLightData.innerCos = glm::cos(glm::radians(l.innerAngleDeg));
			spotLightData.outerCos = glm::cos(glm::radians(l.outerAngleDeg));
			lightData.spotLights.emplace_back(spotLightData);
		}

		auto pointLights = world.GetGroup<PointLightComponent, TransformComponent>();
		lightData.pointLights.reserve(pointLights.size());
		for (auto&& [ent, l, t] : pointLights.each())
		{
			GPUPointLight pointLightData{};
			pointLightData.position = Vec4f(t.GetWorldPosition(), 1.0f);
			pointLightData.lightColor = l.color;
			pointLightData.intensity = l.intensity;
			pointLightData.radius = l.radius;
			lightData.pointLights.emplace_back(pointLightData);
		}

		return lightData;
	}

	RenderContext ExtractContext(World& world)
	{
		RenderContext context{};
		context.camDatas   = CollectCameraData(world);
		context.modelDatas = CollectModelData(world);
		context.lightData  = CollectLightData(world);
		return context;
	}
}
