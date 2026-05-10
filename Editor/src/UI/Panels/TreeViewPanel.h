#pragma once
#include "UI/EditorPanel.h"
#include "Components/CTransform.h"
#include "Components/CName.h"
#include "Entity.h"
#include "ImportedAsset.h"
#include <vector>

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
		std::vector<Entity> EntitiesToDelete;
		bool doCreateNew = false;
		
		bool IsDropped = false;
		size_t NewIndex = 0;

		Entity DraggedEntity = Entity::Invalid();
		Entity NewEntityParent= Entity::Invalid();
		World* gameWorld = nullptr;

		ImportedAsset* DroppedAsset = nullptr;
		Entity AssetDropParent = Entity::Invalid();
	};


	class TreeViewPanel :public EditorPanel
	{
	public:
		TreeViewPanel() :EditorPanel("Tree View") {}

		virtual void PaintContent()override;

		void SetNodeBackgroundColor() //TODO: not for here, part of some style functionality
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_HeaderHovered] = ImGui::ColorConvertU32ToFloat4(col_hovered);
			style.Colors[ImGuiCol_HeaderActive] = ImGui::ColorConvertU32ToFloat4(col_selected);
		}

	private:

		void RenderTreeNode(Entity entity, TreeViewToken& token);
		void RenderDropZone(Entity entity, TreeViewToken& token, bool isAfter);
		void HandleChanges(TreeViewToken& token);

	private:

		const std::string DRAG_TREE_TRANSFORM = "drag_tree_transform";
		const ImU32 SELECTED_COLOR = IM_COL32(80, 120, 200, 80);
		float CORNER_ROUND = 4.0f;
		Entity m_contextMenuEntity = Entity::Invalid();
	};
}
