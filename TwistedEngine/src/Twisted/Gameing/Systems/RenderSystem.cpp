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

namespace Twisted
{
	const std::string MODEL_MATRIX_NAME = "modelMat";
	const std::string VIEW_MATRIX_NAME = "viewMat";
	const std::string PROJ_MATRIX_NAME = "projMat";

	RenderSystem::RenderSystem(World* world) :SystemBase(world)
	{
		//m_renderLayer = m_world->GetApplication()->GetLayer<RenderLayer>();
		//m_renderLayer=
	}

	void RenderSystem::Update()
	{
		const auto cameras = m_world->GetGroup<CCamera>();
		auto renderers = m_world->GetGroup<CRenderer, CTransform>();

		for (const auto camEnt : cameras)
		{
			CCamera& cam = cameras.get<CCamera>(camEnt);

			FrameBuffer* frameBuffer = cam.GetFrameBuffer();
			if (!frameBuffer)
				continue;

			frameBuffer->Bind();
			frameBuffer->ClearBuffers();

			for (auto&& [ent,r,t] : renderers.each())
			{
				Material* material = r.GetSharedMaterial();
				Mesh* mesh = r.GetSharedMesh();
				if (!material || !mesh)
					continue;
				Shader* shader = material->GetShader();
				if (!shader)
					continue;

				material->Set<Mat4x4f>(MODEL_MATRIX_NAME, t.GetWorldModelMatrix());
				material->Set<Mat4x4f>(VIEW_MATRIX_NAME, cam.GetViewMatrix());
				material->Set<Mat4x4f>(PROJ_MATRIX_NAME, cam.GetProjectionMatrix());

				RenderSystemEntry entry;
				entry.framebuffer = frameBuffer;
				entry.material = material;
				entry.mesh = mesh;

				m_renderLayer->SubmitEntry(entry);

			}
		}
	}
};

REGISTER_SYSTEM(RenderSystem,"RenderSystem")