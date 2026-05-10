#include "GameViewPanel.h"

#include "EditorApp/EditorRegistry.h"

#include "OpenGL/FrameBuffer_OpenGL.h"
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

	void GameViewPanel::PaintContent()
	{
		if (World* gameWorld = Application::GetInstance().GetService<GameService>()->GetGameWorld(); gameWorld)
			if (CameraComponent* mainCamera = gameWorld->ForceGetManager<CameraManager>().GetMainCamera(); mainCamera)
				if (Framebuffer* fb = mainCamera->Fb.get(); fb)
				{
					fb->SetSize(Size);
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
