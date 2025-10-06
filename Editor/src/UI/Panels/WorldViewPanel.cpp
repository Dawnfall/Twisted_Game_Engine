#include "WorldViewPanel.h"

#include "Twisted/Application/Application.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "EditorLayer.h"

namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel(EditorLayer* editor) :EditorPanel(editor, "World View")
	{
		m_framebuffer = editor->EditorViewBuffer;
		this->PanelResizeEvent.AddListener([this, editor]() {
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