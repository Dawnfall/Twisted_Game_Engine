#include "UI/Panels/ProjectSettingsPanel.h"

#include "EditorApp/EditorRegistry.h"
#include "Application/Application.h"
#include "AssetsService.h"
#include "ProjectConfig.h"

#include <imgui.h>

namespace Twisted::Editor
{
	void ProjectSettingsPanel::PaintContent()
	{
		auto* assetsService = Application::GetInstance().GetService<AssetsService>();
		if (!assetsService->GetProject().IsValid())
			return;

		ProjectConfig& config = assetsService->GetProject().GetConfig();

		ImGui::SeparatorText("Ambient Light");
		ImGui::ColorEdit3("Color",     &config.ambientLight[0]);
		ImGui::DragFloat ("Intensity", &config.ambientLight.w, 0.001f, 0.0f, 1.0f);

		float footerH = ImGui::GetFrameHeightWithSpacing();
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - footerH);
		if (ImGui::Button("Save", ImVec2(-1, 0)))
			config.Save();
	}
}

REGISTER_EDITOR_PANEL(ProjectSettingsPanel)
