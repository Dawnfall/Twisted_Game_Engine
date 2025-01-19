#pragma once
#include "EditorPanel.h"

namespace Twisted
{
	class CTransform;
	class CRenderer;
}

namespace Twisted::Editor
{
	class EditorApp;

	class DetailsPanel :public EditorPanel
	{
	public:
		DetailsPanel(EditorApp* editor):EditorPanel(editor){}

		virtual void RenderContent(EditorApp* editor)override;
		virtual std::string GetName()override { return "Details Panel"; }

		void RenderTransform(CTransform* transform);
		void RenderRenderer(CRenderer* renderer);
	};
}