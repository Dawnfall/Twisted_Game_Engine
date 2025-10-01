#pragma once
#include "UI/EditorPanel.h"
#include "EditorLayer.h"
#include "Twisted/Gameing/World.h"

namespace Twisted
{
	class AComponent;
}

namespace Twisted::Editor
{
	class DetailsPanel :public EditorPanel
	{
	public:
		DetailsPanel(EditorLayer* editor) :EditorPanel(editor,"Details Panel") {}
		virtual void PaintContent()override;

	private:
		void PaintEntity(EntityID id,World* world);
		
		const std::string ADD_COMPONENT_POPUP = "AddComponentPopup";
		const std::string ADD_COMPONENT_TEXT = "AddComponent";
	};
}