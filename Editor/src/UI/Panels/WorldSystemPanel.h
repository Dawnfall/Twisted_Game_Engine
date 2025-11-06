#pragma once
#include "UI/EditorPanel.h"

namespace Twisted::Editor
{
	class WorldSystemPanel :public EditorPanel
	{
	public:
		WorldSystemPanel() :EditorPanel("Systems Panel") {}
		void PaintContent()override;
	};
}