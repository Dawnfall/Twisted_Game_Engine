#pragma once

#include "UI/EditorPanel.h"

#include "FrameBuffer.h"

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
