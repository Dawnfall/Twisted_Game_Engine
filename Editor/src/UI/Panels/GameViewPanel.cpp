#include "GameViewPanel.h"

#include "Twisted/Application/Application.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "EditorRegistry.h"


namespace Twisted::Editor
{

	GameViewPanel::GameViewPanel() :EditorPanel("Game View")
	{
		this->PanelResizeEvent.AddListener([this]() {
			if (m_gameViewFrameBuffer)
				m_gameViewFrameBuffer->SetSize(Size);
			}
		);
	}

	void GameViewPanel::Init()
	{
		m_gameViewFrameBuffer = RenderLayer::GetInstance()->CreateFrameBuffer("main", Size);
	}
	void GameViewPanel::PaintContent()
	{
		if (m_gameViewFrameBuffer && m_gameViewFrameBuffer->GetTexture())
			ImGui::Image(
				(void*)(intptr_t)m_gameViewFrameBuffer->GetTexture()->GetTexID(),
				ImVec2(Size.x, Size.y),
				ImVec2(0, 1),  // top-left UV
				ImVec2(1, 0)   // bottom-right UV (flipped vertically)
			);
	}
}

REGISTER_EDITOR_PANEL(GameViewPanel)
