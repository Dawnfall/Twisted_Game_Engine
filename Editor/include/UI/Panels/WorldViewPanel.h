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

		virtual void PreRender() override;
		virtual void PaintContent()override;

	private:
		void PaintToolbar(ImVec2 panelPos);
		void HandleShortcuts();

		// Q=View W=Move E=Rotate R=Scale Y=Universal  (mirrors Unity layout)
		enum class GizmoTool { View, Move, Rotate, Scale, Universal };
		void SetTool(GizmoTool tool);

		EditorService* m_editorService = nullptr;
		GizmoTool      m_activeTool    = GizmoTool::Move;
		ImGuizmo::OPERATION m_gizmoOperation = ImGuizmo::TRANSLATE;
		bool m_localSpace = false;
		bool m_toolbarCollapsed = false;
		Vec2i m_pendingFbSize{ 0, 0 };
	};
}
