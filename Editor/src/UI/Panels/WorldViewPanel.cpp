#include "WorldViewPanel.h"

#include "Twisted/Application/Application.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "EditorLayer.h"

namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel(EditorLayer* editor) :EditorPanel(editor, "World View")
	{
		//m_context = m_editor->GetApplication()->GetLayer<RenderLayer>()->CreateNew("worldView", Size.x, Size.y);
		/*this->PanelResizeEvent.AddListener([this, editor]() {
			m_context->frameBuffer->Resize(Size.x, Size.y);
			}
		);*/
	}

	void WorldViewPanel::PaintContent()
	{
		//if (m_context->frameBuffer->GetTexID() != 0)
		//{
		//	ImGui::Image((void*)(intptr_t)m_context->frameBuffer->GetTexID(), ImVec2(Size.x, Size.y));
		//}
	}
}