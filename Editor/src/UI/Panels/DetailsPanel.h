#pragma once

#include "EditorPanel.h"

namespace Twisted
{
	class CTransform;
	class CRenderer;
}

namespace Twisted::Editor
{
	class DetailsPanel :public EditorPanel
	{
		virtual void RenderContent(EditorApp* editor)override;
		virtual std::string GetName()override { return "Details Panel"; }

		void RenderTransform(CTransform* transform);
		void RenderRenderer(CRenderer* renderer);
	};
}