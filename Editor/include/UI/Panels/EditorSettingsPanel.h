#pragma once
#include "UI/EditorPanel.h"

namespace Twisted::Editor
{
	class EditorSettingsPanel : public EditorPanel
	{
	public:
		EditorSettingsPanel() : EditorPanel("Editor Settings") {}
		void PaintContent() override;
	};
}
