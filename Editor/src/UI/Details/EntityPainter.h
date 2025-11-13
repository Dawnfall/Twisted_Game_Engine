#pragma once
#include "EditorRegistry.h"
#include <string>

namespace Twisted::Editor
{
	inline const std::string ADD_COMPONENT_POPUP = "AddComponentPopup";
	inline const std::string ADD_COMPONENT_TEXT = "AddComponent";

	class EntityPainter :public DetailsPainter
	{
	public:
		void Paint(void* obj) override;
	};
}
