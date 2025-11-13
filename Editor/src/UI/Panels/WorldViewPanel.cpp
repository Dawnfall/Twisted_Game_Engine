#include "WorldViewPanel.h"

#include "Twisted/Application/Application.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "EditorRegistry.h"

namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel() :EditorPanel("World View")
	{
		this->PanelResizeEvent.AddListener([this]() {
			if (m_worldViewFrameBuffer)
				m_worldViewFrameBuffer->SetSize(Size);
			}
		);
	}

	void WorldViewPanel::PaintContent()
	{
		if (m_worldViewFrameBuffer && m_worldViewFrameBuffer->GetTexture())
			ImGui::Image((void*)(intptr_t)m_worldViewFrameBuffer->GetTexture()->GetTexID(), ImVec2(Size.x, Size.y));
	}
}

REGISTER_EDITOR_PANEL(WorldViewPanel)
