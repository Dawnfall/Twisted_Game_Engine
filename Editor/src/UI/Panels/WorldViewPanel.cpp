#include "WorldViewPanel.h"
#include "EditorRuntime.h"
#include "Rendering/RenderLayer.h"

namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel(EditorRuntime* editor) :EditorPanel(editor)
	{
		m_context =m_editor->App->GetLayer<rend::RenderLayer>()->CreateNew("worldView", Size.x, Size.y);
		this->PanelResizeEvent.AddListener([this, editor](){
				m_context->frameBuffer->Resize(Size.x, Size.y);
			}
		);
	}

	void WorldViewPanel::RenderContent()
	{
		if (m_context->frameBuffer->GetTexID() != 0)
		{
			ImGui::Image((void*)(intptr_t)m_context->frameBuffer->GetTexID(), ImVec2(Size.x, Size.y));
		}
	}
}