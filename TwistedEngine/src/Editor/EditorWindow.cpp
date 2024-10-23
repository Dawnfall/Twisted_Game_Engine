#include "pch.h"
#include "EditorWindow.h"
#include "Twisted/Application.h"

namespace Twisted::Editor
{
	void EditorWindow::Render(Application* app)
	{
		ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_FirstUseEver);

		ImGui::Begin(GetName().c_str());
		RenderContent(app);
		ImGui::End();
	}
}
