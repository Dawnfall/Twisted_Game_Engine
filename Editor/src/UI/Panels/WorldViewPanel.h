#pragma once
#include "UI/EditorPanel.h"
#include "Twisted/Rendering/RenderContext.h"

#include <string>

namespace Twisted::Editor
{
	class WorldViewPanel :public EditorPanel
	{
	public:
		WorldViewPanel(EditorLayer* editor);

		virtual void PaintContent()override;

		RenderContext* m_context = nullptr;
	};
}