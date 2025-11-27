#include "WorldViewPanel.h"

#include "Twisted/Application/Application.h"
#include "EditorApp/EditorRegistry.h"

#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"

namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel() :EditorPanel("World View")
	{
		this->PanelResizeEvent.AddListener([this]() {
			auto fb = EditorWorldService::GetInstance()->renderer.EditorFrameBuffer;
			if (fb)
				FrameBuffer_GL::SetSize(*fb, Size);
			}
		);
	}

	void WorldViewPanel::PaintContent()
	{
		auto fb = EditorWorldService::GetInstance()->renderer.EditorFrameBuffer;
		if (fb && fb->Tex)
			ImGui::Image(
				(void*)(intptr_t)fb->Tex->TexID, 
				ImVec2(Size.x, Size.y),
				ImVec2(0, 1),  // top-left UV
				ImVec2(1, 0)   // bottom-right UV (flipped vertically)
			);
	}
}

REGISTER_EDITOR_PANEL(WorldViewPanel)
