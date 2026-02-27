#include "GameViewPanel.h"

#include "EditorApp/EditorRegistry.h"

#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
#include "Twisted/Gameing/GameService.h"
#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/TObject.h"
#include "UI/EditorPanel.h"
#include <imgui.h>

#include "Twisted/Gameing/Managers/CameraManager.h"
#include "Twisted/Gameing/Components/CCamera.h"

#include "UI/ImguiExtensions.h"

namespace Twisted::Editor
{

	GameViewPanel::GameViewPanel() :EditorPanel("Game View")
	{
		this->PanelResizeEvent.AddListener([this]() {
			if (World* gameWorld = GameService::GetInstance()->GameWorld; gameWorld)
				if (CameraComponent* mainCamera = gameWorld->GetManager<CameraManager>()->GetMainCamera(); mainCamera)
					if (Framebuffer* fb = mainCamera->Fb.get(); fb)
					{
						fb->SetSize(Size);
					}
			});
	}

	void GameViewPanel::Init()
	{

	}

	void GameViewPanel::PaintContent()
	{
		if (World* gameWorld = GameService::GetInstance()->GameWorld; gameWorld)
			if (CameraComponent* mainCamera = gameWorld->GetManager<CameraManager>()->GetMainCamera(); mainCamera)
				if (Framebuffer* fb = mainCamera->Fb.get(); fb)
				{
					//ImGui::Image(
					//	 Im::GetImGuiTextureID(fb->GetColor().get()),
					//	ImVec2((float)Size.x, (float)Size.y),
					//	ImVec2(0, 1),  // top-left UV
					//	ImVec2(1, 0)   // bottom-right UV (flipped vertically)
					//);
				}
	}
}

REGISTER_EDITOR_PANEL(GameViewPanel)
