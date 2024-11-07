#pragma once
#include "editorpch.h"

namespace Twisted::Editor
{
	class EditorApp;

	class EditorPanel
	{
	public:
		virtual void RenderContent(EditorApp* editor);
		virtual std::string GetName() { return "New Panel"; }

		bool IsShowing = true;
		ImGuiID DockParentID = -1;
	};
}