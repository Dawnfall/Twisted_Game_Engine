#pragma once
#include "UI/EditorPanel.h"
#include "EditorApp/EditorService.h"
#include "Application/Application.h"
#include <ImGuizmo.h>
#include <imgui.h>

namespace Twisted::Editor
{
	class WorldViewPanel :public EditorPanel
	{
	public:
		WorldViewPanel();
		void Init() override
		{
			m_editorService = Application::GetInstance().GetService<EditorService>();
			ImGuizmo::AllowAxisFlip(false);
		}

		virtual void PaintContent()override;

	private:
		void PaintToolbar(ImVec2 panelPos);

		EditorService* m_editorService = nullptr;
		ImGuizmo::OPERATION m_gizmoOperation = ImGuizmo::TRANSLATE;
		bool m_localSpace = false;
		bool m_toolbarCollapsed = false;
	};
}
