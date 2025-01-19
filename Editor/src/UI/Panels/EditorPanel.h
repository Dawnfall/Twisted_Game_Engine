#pragma once
#include "Utils/Event.h"

namespace Twisted
{
	class World;
}

namespace Twisted::Editor
{
	class EditorApp;
	class EditorPanel
	{
	public:
		EditorPanel(EditorApp* editor) {}
		virtual void RenderContent(EditorApp* editor) = 0;
		virtual std::string GetName() = 0;

		Event<> PanelResizeEvent;

		bool IsShowing = true;
		ImGuiID DockParentID = -1;
		ImVec2 Size;
	};
}