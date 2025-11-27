#pragma once
#include "UI/EditorPanel.h"

#include "Twisted/Rendering/FrameBuffer.h"
#include "EditorApp/EditorWorldService.h"

namespace Twisted::Editor
{
	class WorldViewPanel :public EditorPanel
	{
	public:
		WorldViewPanel();
		void Init() override
		{		
			EditorWorldService::GetInstance()->renderer.EditorFrameBuffer = TObject::Create<FrameBuffer>("World framebuffer",Size);
		}

		virtual void PaintContent()override;
	};
}
