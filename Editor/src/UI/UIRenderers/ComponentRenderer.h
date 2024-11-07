#pragma once
#include "Twisted/Game/AComponent.h"

namespace Twisted::Editor
{
	template<typename T>
	class UIRenderer
	{
		void Render(T* element){}
	};

	
	class ComponentRenderer :UIRenderer<AComponent>
	{
		void Render(AComponent* component){}
	};

	template<typename T>
	void RenderBase(const char* label, T& value) {
		ImGui::Text("%s", label);
	}
}