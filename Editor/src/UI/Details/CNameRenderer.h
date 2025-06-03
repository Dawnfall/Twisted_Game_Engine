#pragma once
#include "DetailsRenderer.h"
#include "Twisted/Game/Components/CName.h"
#include "imgui.h"
#include "UI/ImguiExtensions.h"

namespace Twisted::Editor
{
	template<>
	class DetailsRenderer<CName>
	{
	public:
		void RenderDetails(CName* name)
		{
			std::string nameStr = name->GetName();
			if (Im::InputText("Name##Name", nameStr))
				name->SetName(nameStr);
		}

		std::string GetName() { return "Name"; }
	};
}