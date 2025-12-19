#pragma once
#include "Utils/Event.h"
#include "Utils/GlmUtils.h"
#include "imgui.h"

namespace Twisted
{
	class World;
}

namespace Twisted::Editor
{
	class EditorPanel
	{
	public:
		EditorPanel(const std::string& name) : m_panelName(name) {}

		virtual void Init(){}
		virtual void PaintContent() = 0;

		const std::string& GetName()const { return m_panelName; }

		Event<> PanelResizeEvent;

		bool IsShowing = true;
		ImGuiID DockParentID = -1;
		Vec2i Size = { 10,10 };

		bool IsInit = false;
	private:
		std::string m_panelName;
	};
}
