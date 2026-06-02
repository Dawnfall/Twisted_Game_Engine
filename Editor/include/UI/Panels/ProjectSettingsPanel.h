#pragma once
#include "UI/EditorPanel.h"

namespace Twisted::Editor
{
	class ProjectSettingsPanel : public EditorPanel
	{
	public:
		ProjectSettingsPanel() : EditorPanel("Project Settings") {}
		void PaintContent() override;
	};
}
