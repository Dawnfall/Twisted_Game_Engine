#pragma once
#include "UI/EditorPanel.h"
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
		DetailsPanel() :EditorPanel("Details Panel") {}
		virtual void PaintContent()override;
	};
}