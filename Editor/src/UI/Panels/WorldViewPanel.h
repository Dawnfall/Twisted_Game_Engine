#pragma once
#include "UI/EditorPanel.h"

#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/Rendering/RenderLayer.h"

namespace Twisted::Editor
{
	class WorldViewPanel :public EditorPanel
	{
	public:
		WorldViewPanel();
		void Init() override
		{		
			m_worldViewFrameBuffer = RenderLayer::GetInstance()->CreateFrameBuffer("World view", Size);
		}

		virtual void PaintContent()override;
		FrameBuffer* m_worldViewFrameBuffer = nullptr;

	};
}
