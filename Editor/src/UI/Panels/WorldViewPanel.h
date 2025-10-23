#pragma once
#include "UI/EditorPanel.h"
#include <string>

#include "Twisted/Rendering/FrameBuffer.h"
#include "Utils/WPtr.h"

namespace Twisted::Editor
{
	class WorldViewPanel :public EditorPanel
	{
	public:
		WorldViewPanel();

		virtual void PaintContent()override;

		void SetFrameBuffer(FrameBuffer* framebuffer) { m_framebuffer = framebuffer; }

		WPtr<FrameBuffer> m_framebuffer;
	};
}