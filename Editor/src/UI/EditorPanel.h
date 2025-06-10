#pragma once
#include "Data/Event.h"
#include "Utils/GlmUtils.h"
#include "imgui.h"

namespace Twisted
{
	class World;
}

namespace Twisted::Editor
{
	class EditorRuntime;
	class EditorPanel
	{
	public:
		EditorPanel(EditorRuntime* editor) :m_editor(editor) {}
		virtual void RenderContent() = 0;
		virtual std::string GetName() = 0;

		Event<> PanelResizeEvent;

		bool IsShowing = true;
		ImGuiID DockParentID = -1;
		Vec2i Size = { 10,10 };
		EditorRuntime* m_editor;
	};
}