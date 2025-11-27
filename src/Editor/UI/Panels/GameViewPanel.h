#pragma once

#include "UI/EditorPanel.h"

#include "Twisted/Rendering/FrameBuffer.h"

namespace Twisted::Editor
{
	class GameViewPanel :public EditorPanel
	{
	public:
		GameViewPanel();
		void Init() override;

		virtual void PaintContent()override;
	};

}
