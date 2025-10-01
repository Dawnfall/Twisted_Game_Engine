#pragma once
#include "Twisted/Data/Event.h"
#include "Utils/GlmUtils.h"
#include "imgui.h"

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
		EditorPanel(EditorLayer* editor,const std::string& name) :m_editor(editor), m_panelName(name) {}
		virtual void PaintContent() = 0;

		const std::string& GetName()const { return m_panelName; }

		Event<> PanelResizeEvent;

		bool IsShowing = true;
		ImGuiID DockParentID = -1;
		Vec2i Size = { 10,10 };
		EditorLayer* m_editor;

		bool IsInit = false;
	private:
		std::string m_panelName;
	};
}