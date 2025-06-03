#pragma once
#include "Utils/Event.h"
#include "Utils/GlmUtils.h"

namespace Twisted
{
	class World;
}

namespace Twisted::Editor
{
	class EditorLayer;
	class EditorPanel
	{
	public:
		EditorPanel(EditorLayer* editor) :editor(editor) {}
		virtual void RenderContent() = 0;
		virtual std::string GetName() = 0;

		Event<> PanelResizeEvent;

		bool IsShowing = true;
		ImGuiID DockParentID = -1;
		Vec2i Size = { 0,0 };
		EditorLayer* editor;
	};
}