#include "GameViewPanel.h"

#include "Twisted/Rendering/RenderLayer.h"
#include "EditorRegistry.h"

#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"

namespace Twisted::Editor
{

	GameViewPanel::GameViewPanel() :EditorPanel("Game View")
	{
		this->PanelResizeEvent.AddListener([this]() {
			if (m_gameViewFrameBuffer)
				FrameBuffer_GL::SetSize(*m_gameViewFrameBuffer, Size);
			}
		);
	}

	void GameViewPanel::Init()
	{
		m_gameViewFrameBuffer = RenderLayer::GetInstance()->CreateFrameBuffer("main", Size);
	}
	void GameViewPanel::PaintContent()
	{
		if (m_gameViewFrameBuffer && m_gameViewFrameBuffer->Tex)
			ImGui::Image(
				(void*)(intptr_t)m_gameViewFrameBuffer->Tex->TexID,
				ImVec2(Size.x, Size.y),
				ImVec2(0, 1),  // top-left UV
				ImVec2(1, 0)   // bottom-right UV (flipped vertically)
			);
	}
}

REGISTER_EDITOR_PANEL(GameViewPanel)
