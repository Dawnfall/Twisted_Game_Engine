#pragma once
#include "EditorPanel.h"

namespace Twisted
{
	class World;
}

namespace Twisted::Editor
{
	class TreeViewPanel :public EditorPanel
	{
	public:
		virtual void RenderContent(EditorApp* editor)override;

		virtual std::string GetName()override { return "Tree View Panel"; }
	};
}