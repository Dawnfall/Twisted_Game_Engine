#pragma once
#include "UI/EditorPanel.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/CName.h"
#include "Twisted/Gameing/Entity.h"

namespace Twisted::Editor
{
	// Custom colors
	const ImU32 col_hovered = IM_COL32(70, 70, 70, 180);   // subtle dark gray
	const ImU32 col_selected = IM_COL32(60, 90, 170, 200);  // bluish highlight
	const std::string nodeContextPopup = "NodeContextPopup";
	const std::string emptyContextPopup = "EmptyContextPopup";


	struct TreeViewToken
	{
		bool IsClickUsed = false;
		ImVec2 EntireRegion;

		Entity EntityToDelete;

		bool doCreateNew = false;
		Entity NewEntityParent;
		//TODO: meshentity
	};


	class TreeViewPanel :public EditorPanel
	{
	public:
		TreeViewPanel(EditorLayer* editor) :EditorPanel(editor,"Tree View") {}

		virtual void PaintContent()override;

		void SetNodeBackgroundColor() //TODO: not for here, part of some style functionality
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_HeaderHovered] = ImGui::ColorConvertU32ToFloat4(col_hovered);
			style.Colors[ImGuiCol_HeaderActive] = ImGui::ColorConvertU32ToFloat4(col_selected);
		}

		ImGuiTreeNodeFlags GetNodeFlags(size_t childCount, bool isSelected)
		{
			ImGuiTreeNodeFlags flags =
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_SpanAvailWidth;

			if (childCount == 0)
				flags |= ImGuiTreeNodeFlags_Leaf;

			if (isSelected)
				flags |= ImGuiTreeNodeFlags_Selected;

			return flags;
		}
	private:

		void RenderTreeNode(Entity entity, TreeViewToken& token);
		void RenderDropZone(Entity entity, TreeViewToken& token, bool isAfter);
		void HandleChanges(TreeViewToken& token);

		void RightClickEmpty(TreeViewToken& token);
		void RightClickOnNode(Entity entity, TreeViewToken& token);
		void LeftClickOnNode(Entity entity, bool isSelected);
		void DragDrop(Entity entity, size_t childCount, const std::string& name);
	private:
		bool m_isDroped = false;
		size_t m_newIndex = 0;
		Entity newParentEntity;
		Entity draggedEntity;

		const std::string DRAG_TREE_TRANSFORM = "drag_tree_transform";
		const ImU32 SELECTED_COLOR = IM_COL32(80, 120, 200, 80);
		float CORNER_ROUND = 4.0f;
	}; 
}