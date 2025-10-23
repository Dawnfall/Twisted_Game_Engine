#include "WorldViewPanel.h"

#include "EditorMacros.h"

#include "Twisted/Application/Application.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "EditorRegistry.h"
#include "EditorData/EditorData.h"
namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel() :EditorPanel("World View")
	{
		m_framebuffer = EditorData::GetInstance().EditorViewBuffer;
		this->PanelResizeEvent.AddListener([this]() {
			if (m_framebuffer)
				m_framebuffer->SetSize(Size);
			}
		);
	}

	void WorldViewPanel::PaintContent()
	{
		if (m_framebuffer && m_framebuffer->GetTexture())
			ImGui::Image((void*)(intptr_t)m_framebuffer->GetTexture()->GetTexID(), ImVec2(Size.x, Size.y));
	}
}

REGISTER_EDITOR_PANEL(WorldViewPanel)