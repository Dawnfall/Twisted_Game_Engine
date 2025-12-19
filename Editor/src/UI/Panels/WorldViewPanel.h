#pragma once
#include "UI/EditorPanel.h"

#include "Twisted/Rendering/FrameBuffer.h"
#include "EditorApp/EditorWorldService.h"
#include "ImGuizmo.h"
#include "Twisted/Gameing/Components/CCamera.h"

namespace Twisted::Editor
{
	class WorldViewPanel :public EditorPanel
	{
	public:
		WorldViewPanel();
		void Init() override
		{		
			m_editorWorld = EditorWorldService::GetInstance();
			m_editorWorld->renderer.EditorFrameBuffer = TObject::Create<FrameBuffer>("World framebuffer",Size);
		}

		virtual void PaintContent()override;

    private:
		void DrawViewportGizmo(Entity camEntity);

		EditorWorldService* m_editorWorld=nullptr;
	};
}
