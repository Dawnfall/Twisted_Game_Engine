#include "UI/Panels/GameViewPanel.h"

#include "EditorApp/EditorRegistry.h"

#include "GameService.h"
#include "FrameBuffer.h"
#include "Application/TObject.h"
#include "UI/EditorPanel.h"
#include <imgui.h>

#include "Managers/CameraManager.h"
#include "Components/CCamera.h"
#include "Application/Application.h"
#include "UI/ImguiExtensions.h"

namespace Twisted::Editor
{

	GameViewPanel::GameViewPanel() :EditorPanel("Game View")
	{
	}

	void GameViewPanel::Init()
	{

	}

	void GameViewPanel::PreRender()
	{
		if (m_pendingFbSize.x <= 0 || m_pendingFbSize.y <= 0) return;
		if (World* gameWorld = Application::GetInstance().GetService<GameService>()->GetGameWorld(); gameWorld)
			if (CameraComponent* mainCamera = gameWorld->ForceGetManager<CameraManager>().GetMainCamera(); mainCamera)
				if (Framebuffer* fb = mainCamera->Fb.get(); fb)
					fb->SetSize(m_pendingFbSize);
		m_pendingFbSize = { 0, 0 };
	}

	void GameViewPanel::PaintContent()
	{
		// Request FB resize for next frame's PreRender (safe — no command buffer active then).
		if (Size.x > 0 && Size.y > 0)
			m_pendingFbSize = Size;

		if (World* gameWorld = Application::GetInstance().GetService<GameService>()->GetGameWorld(); gameWorld)
			if (CameraComponent* mainCamera = gameWorld->ForceGetManager<CameraManager>().GetMainCamera(); mainCamera)
				if (Framebuffer* fb = mainCamera->Fb.get(); fb && fb->IsValid())
				{
					// fb->SetSize(Size);  // moved to PreRender() — unsafe here while command buffer is active
					ImGui::Image(
						Im::GetImGuiTextureID(fb->GetColor().get()),
						ImVec2((float)Size.x, (float)Size.y),
						ImVec2(0, 1),  // top-left UV
						ImVec2(1, 0)   // bottom-right UV (flipped vertically)
					);
				}
	}
}

REGISTER_EDITOR_PANEL(GameViewPanel)
