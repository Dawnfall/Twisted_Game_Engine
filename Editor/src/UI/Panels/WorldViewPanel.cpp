#include "WorldViewPanel.h"

#include "EditorApp/EditorRegistry.h"
#include "UI/ImguiExtensions.h"

#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/Managers/CameraManager.h"

#include <imgui.h>

namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel() :EditorPanel("World View")
	{
		this->PanelResizeEvent.AddListener([this]() {
			if (!m_editorService) return;
			auto* camera = m_editorService->GetEditorWorld()->ForceGetManager<CameraManager>().GetMainCamera();
			if (!camera)
				return;
			if (auto* fb = camera->Fb.get())
				fb->SetSize(Size);
			if (Size.y > 0)
				camera->AspectRatio = (float)Size.x / (float)Size.y;
			}
		);
	}

	void WorldViewPanel::PaintContent()
	{
		if (!m_editorService)
			return;

		CameraComponent* camera = m_editorService->GetEditorWorld()->ForceGetManager<CameraManager>().GetMainCamera();
		if (!camera)
			return;

		Framebuffer* fb = camera->Fb.get();
		if (!fb || !fb->GetColor())
			return;

		ImGui::Image(
			Im::GetImGuiTextureID(fb->GetColor().get()),
			ImVec2((float)Size.x, (float)Size.y),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);
	}
}

REGISTER_EDITOR_PANEL(WorldViewPanel)
