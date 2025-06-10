#pragma once
#include "UI/EditorPanel.h"
#include "Game/Components/CTransform.h"

namespace Twisted::Editor
{
	class TreeViewPanel :public EditorPanel
	{
	public:
		TreeViewPanel(EditorRuntime* editor) :EditorPanel(editor) {}
		virtual void RenderContent()override;
		virtual std::string GetName()override { return "Tree View"; }

	private:
		int m_siblingIndex = -1;
		EntityID m_draggedEnt = NullEntity;
		EntityID m_dropedOnEnt = NullEntity;

		void RenderTreeNode(EntityID entity);
		void RenderDropZone(EntityID entity, unsigned int position, bool isAfter);
	};
}