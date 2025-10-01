#pragma once

#include "UI/Details/ComponentPainter.h"
#include "Twisted/Gameing/Components/CName.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/World.h"


namespace Twisted::Editor
{
	class CNamePainter :public ComponentPainter
	{
	public:
		void AddComponent(EntityID entity, World* world) override { }
		void* GetComponent(EntityID ent, World* world)const override { return world->TryGetComponent<CName>(ent); }
		std::string GetComponentName()const override { return "Name"; }

		virtual void Paint(void* obj)override
		{
			CName* name = static_cast<CName*>(obj);

			Im::InputTextToken token;

			token.DoAutoFocus = false;
			token.PreLabel = "Name";
			token.Text = name->GetName();

			if (Im::InputText(token))
				name->SetName(token.Text);
		}

	};
}