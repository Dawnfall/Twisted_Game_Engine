#pragma once

#include "UI/ComponentPainter.h"
#include "Twisted/Gameing/Components/CRenderer.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/World.h"

namespace Twisted::Editor
{
	class CRendererPainter :public ComponentPainter
	{
	public:
		void AddComponent(Entity entity) override { entity.GetWorld()->AddComponent<CRenderer>(entity.GetID()); }
		void* GetComponent(Entity entity) override { return entity.GetWorld()->TryGetComponent<CRenderer>(entity.GetID()); }
		std::string GetComponentName()const override { return "Renderer"; }

		virtual void Paint(void* obj)override
		{
			CRenderer* renderer = static_cast<CRenderer*>(obj);

			//Im::ComponentDropField<Mesh>();
			//Im::ObjectDropField<Material>("Material", renderer->GetSharedMaterial(), nullptr);
			//Im::ObjectDropField<Mesh>("Mesh", renderer->GetSharedMesh(), nullptr);
		}
	};
}