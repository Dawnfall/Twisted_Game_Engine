#include "RenderSystem.h"

//#include "Twisted/Rendering/RenderingAPI.h"

#include "Twisted/Gameing/World.h"
#include "Twisted/Gameing/Components/CRenderer.h"
#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/Components/CTransform.h"

#include "Twisted/Gameing/WorldRegistry.h"

//#include "Twisted/Rendering/Mesh.h"
//#include "Twisted/Rendering/Material.h"
//#include "Twisted/Rendering/RenderContext.h"
#include "Twisted/Rendering/Data/RenderContext.h"

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
		const auto cameras = m_world->GetGroup<CCamera>();
		context.camDatas.reserve(cameras.size());
		for (const auto camEnt : cameras)
		{
			CCamera& cam = cameras.get<CCamera>(camEnt);
			CameraData data{};
			if (cam.IsMainCamera())
			{
				data.framebuffer = m_renderLayer->GetFrameBuffer("main");
			}
			else
			{
				data.framebuffer = cam.GetFrameBuffer();
				if (!data.framebuffer)
					continue;
			}
			data.viewMatrix = cam.GetViewMatrix();
			data.projectionMatrix = cam.GetProjectionMatrix();

			context.camDatas.emplace_back(data);
		}

		auto renderers = m_world->GetGroup<CRenderer, CTransform>();
		context.modelDatas.reserve(renderers.size());
		for (auto&& [ent, r, t] : renderers.each())
		{
			ModelData data{};
			data.material = r.GetSharedMaterial();
			data.mesh = r.GetSharedMesh();

			if (!data.material || !data.mesh || !data.material->GetShader())
				continue;

			data.modelMatrix = t.GetWorldModelMatrix();
			context.modelDatas.emplace_back(data);
		}

		m_renderLayer->SubmitEntry(std::move(context));
	}
};

REGISTER_SYSTEM(RenderSystem, "RenderSystem")

