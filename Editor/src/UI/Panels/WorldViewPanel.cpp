#include "editorpch.h"
#include "WorldViewPanel.h"
//#include "Twisted/Managers/WindowManager.h"
//#include "Twisted/Game/Components/CCamera.h"
//
namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel(EditorLayer* editor) :EditorPanel(editor), m_context(Size.x, Size.y)
	{
		//		this->PanelResizeEvent.AddListener([this, editor]()
		//			{
		//				//m_context.frameBuffer->Resize(Size.x, Size.y);
		//			}
		//		);
	}
	//
	void WorldViewPanel::RenderContent()
	{
		//		const auto& cameraEntts = editor->GetActiveWorld()->GetComponents<Twisted::CCamera>();
		//		if (!cameraEntts.empty())
		//		{
		//			auto& camera = cameraEntts.get<Twisted::CCamera>(cameraEntts[0]);
		//			if (m_context.frameBuffer->GetTexID() != 0)
		//			{
		//				GLuint textureID = m_context.frameBuffer->GetTexID();
		//				//ImGui::Image((void*)(intptr_t)textureID, Size);
		//			}
		//		}
	}
	//
}