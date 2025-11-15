#include "WorldViewPanel.h"

#include "Twisted/Application/Application.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "EditorRegistry.h"

#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel() :EditorPanel("World View")
	{
		this->PanelResizeEvent.AddListener([this]() {
			if (m_worldViewFrameBuffer)
				FrameBuffer_GL::SetSize(*m_worldViewFrameBuffer, Size);
			}
		);
	}

	void WorldViewPanel::PaintContent()
	{
		if (m_worldViewFrameBuffer && m_worldViewFrameBuffer->Tex)
			ImGui::Image((void*)(intptr_t)m_worldViewFrameBuffer->Tex->TexID, ImVec2(Size.x, Size.y));
	}
}

//REGISTER_EDITOR_PANEL(WorldViewPanel)
