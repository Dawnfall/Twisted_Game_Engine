#include "editorpch.h"
#include "WorldViewPanel.h"
#include "EditorApp.h"

namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel(EditorApp* editor) :EditorPanel(editor)
	{
		this->PanelResizeEvent.AddListener([this,editor]()
			{
				editor->GetFrameBuffer()->Resize(static_cast<GLsizei>(this->Size.x), static_cast<GLsizei>(this->Size.y));
			}
		);
	}

	void WorldViewPanel::RenderContent(EditorApp* editor)
	{
		GLuint textureID = editor->GetFrameBuffer()->GetTexID();

		ImGui::Image((void*)(intptr_t)textureID, Size);
	}

}