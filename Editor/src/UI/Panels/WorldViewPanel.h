#pragma once
#include "EditorPanel.h"
#include <string>

namespace Twisted::Editor
{

	class WorldViewPanel :public EditorPanel
	{
	public:
		WorldViewPanel(EditorApp* editor);

		virtual void RenderContent(EditorApp* editor)override;
		virtual std::string GetName()override { return "World View"; }
	};

}