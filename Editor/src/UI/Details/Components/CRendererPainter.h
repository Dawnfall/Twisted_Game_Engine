#pragma once

#include "UI/Details/ComponentPainter.h"
#include "Twisted/Gameing/Components/CRenderer.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/World.h"


namespace Twisted::Editor
{
	class CRendererPainter :public ComponentPainter
	{
	public:
		void AddComponent(EntityID entity, World* world) override { world->AddComponent<CRenderer>(entity); }
		void* GetComponent(EntityID ent, World* world)const override { return world->TryGetComponent<CRenderer>(ent); }
		std::string GetComponentName()const override { return "Renderer"; }

		virtual void Paint(void* obj)override
		{
			CRenderer* renderer = static_cast<CRenderer*>(obj);

			//Im::ComponentDropField<Mesh>();
			Im::ObjectDropField<Material>("Material", renderer->GetSharedMaterial(), nullptr);
			Im::ObjectDropField<Mesh>("Mesh", renderer->GetSharedMesh(), nullptr);
		}

	};
}