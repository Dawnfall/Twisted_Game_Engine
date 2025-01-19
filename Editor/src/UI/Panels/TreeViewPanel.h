#pragma once
#include "EditorPanel.h"
#include "Twisted/Game/Components/CTransform.h"

namespace Twisted
{
	class World;
}

namespace Twisted::Editor
{
	class EditorApp;
	class TreeViewPanel :public EditorPanel
	{
	public:
		TreeViewPanel(EditorApp* editor) :EditorPanel(editor) {}
		virtual void RenderContent(EditorApp* editor)override;

		virtual std::string GetName()override { return "Tree View"; }

		static ImGuiTreeNodeFlags SetTreeFlags(const CTransform& transform,EditorApp* app);
	};
}