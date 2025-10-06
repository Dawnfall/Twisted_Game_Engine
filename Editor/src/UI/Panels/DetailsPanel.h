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
	};
}