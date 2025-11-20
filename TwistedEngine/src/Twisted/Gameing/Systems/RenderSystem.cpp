#include "RenderSystem.h"

//#include "Twisted/Rendering/RenderingAPI.h"

#include "Twisted/Gameing/World.h"

#include "Twisted/Gameing/Components/CRenderer.h"
#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/SpotLightComponent.h"
#include "Twisted/Gameing/Components/DirectionalLightComponent.h"
#include "Twisted/Gameing/Components/PointLightComponent.h"

#include "Twisted/Gameing/WorldRegistry.h"
#include "Twisted/Rendering/Data/RenderContext.h"

//#include "Twisted/Rendering/Mesh.h"
//#include "Twisted/Rendering/Material.h"
//#include "Twisted/Rendering/RenderContext.h"

namespace Twisted
{
	RenderSystem::RenderSystem(World* world) :
		SystemBase(world),
		m_renderLayer(RenderLayer::GetInstance())
	{
	}

	void RenderSystem::Update()
	{
		RenderContext context{};

		context.camDatas = CollectCameraData(m_world);
		context.modelDatas = CollectModelData(m_world);
		context.lightData = CollectLightData(m_world);

		m_renderLayer->SubmitEntry(std::move(context));
	}

	std::vector<CameraData> RenderSystem::CollectCameraData(World* world)const
	{
		std::vector<CameraData> camData;

		const auto cameras = world->GetGroup<CameraComponent>();
		camData.reserve(cameras.size());
		for (const auto camEnt : cameras)
		{
			CameraComponent& cam = cameras.get<CameraComponent>(camEnt);
			CameraData data{};
			if (cam.IsMainCamera())
			{
				data.framebuffer = m_renderLayer->GetFrameBuffer("main");
			}
			else
			{
				data.framebuffer = cam.Fb.get();
				if (!data.framebuffer)
					continue;
			}
			data.viewMatrix = cam.GetViewMatrix();
			data.projectionMatrix = cam.GetProjectionMatrix();
			data.clearParams = cam.ClearParams;

			camData.emplace_back(data);
		}

		return camData;
	}

	std::vector<ModelData> RenderSystem::CollectModelData(World* world)const
	{
		std::vector<ModelData> modelData;

		auto renderers = world->GetGroup<CRenderer, CTransform>();
		modelData.reserve(renderers.size());
		for (auto&& [ent, r, t] : renderers.each())
		{
			ModelData data{};
			data.material = r.material;
			data.mesh = r.mesh;

			if (!data.material || !data.mesh || !data.material->Shader)
				continue;

			data.modelMatrix = t.GetWorldModelMatrix();
			modelData.emplace_back(data);
		}

		return modelData;
	}

	LightData RenderSystem::CollectLightData(World* world)const
	{
		LightData lightData;

		auto dirLights = world->GetGroup<DirectionalLightComponent, CTransform>();
		lightData.dirLights.reserve(dirLights.size());
		for (auto&& [ent, l, t] : dirLights.each())
		{
			DirLightData dirLightData;

			dirLightData.direction = Vec4f(t.GetWorldForward(), 0.0f);
			dirLightData.lightColor = l.color;
			dirLightData.intensity = l.intensity;

			lightData.dirLights.emplace_back(dirLightData);
		}

		auto spotLights = world->GetGroup<SpotLightComponent, CTransform>();
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

		auto pointLights = world->GetGroup<PointLightComponent, CTransform>();
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
};

//REGISTER_SYSTEM(RenderSystem, "RenderSystem")

