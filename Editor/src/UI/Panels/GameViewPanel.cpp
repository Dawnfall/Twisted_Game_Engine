#include "GameViewPanel.h"

#include "EditorApp/EditorRegistry.h"

#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
#include "Twisted/Gameing/GameService.h"
#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/TObject.h"
#include "UI/EditorPanel.h"
#include <imgui.h>

namespace Twisted::Editor
{

	GameViewPanel::GameViewPanel() :EditorPanel("Game View")
	{
		this->PanelResizeEvent.AddListener([this]() {
			auto fb = GameService::GetInstance()->renderer.GameFrameBuffer;
			if (fb)
				fb->SetSize(Size);
			}
		);
	}

	void GameViewPanel::Init()
	{
		GameService::GetInstance()->renderer.GameFrameBuffer = TObject::Create<FrameBuffer>("game", Size);
	}
	void GameViewPanel::PaintContent()
	{
		//auto fb = GameService::GetInstance()->renderer.GameFrameBuffer;
		//if (fb && fb->Tex)
		//	ImGui::Image(
		//		(void*)(intptr_t)fb->Tex->TexID,
		//		ImVec2((float)Size.x, (float)Size.y),
		//		ImVec2(0, 1),  // top-left UV
		//		ImVec2(1, 0)   // bottom-right UV (flipped vertically)
		//	);
	}
}

REGISTER_EDITOR_PANEL(GameViewPanel)
