#pragma once

#include <vector>
#include <string>

#include "AppCore.h"
#include "UI/EditorPanel.h" 
#include <imgui.h>

namespace Twisted::Editor
{
	//enum class SplitDirection
	//{
	//	None,   // Leaf node (contains windows)
	//	Left,
	//	Right,
	//	Up,
	//	Down
	//};

	//struct FrameNode
	//{
	//	std::vector<std::string> ToggledPanels; //Leafs Only
	//	size_t activeIndex = 0; //which one is active
	//	SplitDirection Direction = SplitDirection::None;

	//	float SplitRatio = 0.5f; //if split
	//	URef<FrameNode> FirstChild = nullptr;
	//	URef<FrameNode> SecChild = nullptr;
	//};



	class PanelManager
	{
	public:



	private:

	};
}		

//ImGuiID BuildDockSpace(ImGuiID parentID, FrameNode* node)
		//{
		//	if (!node)
		//		return parentID;

		//	if (node->Direction == SplitDirection::None)
		//	{
		//		// Leaf: dock all panels here
		//		for (size_t i = 0; i < node->ToggledPanels.size(); i++)
		//		{
		//			const std::string& panel = node->ToggledPanels[i];
		//			ImGui::DockBuilderDockWindow(panel.c_str(), parentID);

		//			if (i == node->activeIndex)
		//				ImGui::SetWindowFocus(panel.c_str()); // Focus active tab
		//		}
		//		return parentID;
		//	}
		//	else
		//	{
		//		// Split: create children
		//		ImGuiID child_a = 0, child_b = 0;
		//		ImGuiDir dir = (ImGuiDir)node->Direction;
		//		child_b = ImGui::DockBuilderSplitNode(parentID, dir, node->SplitRatio, &child_a, &child_b);

		//		BuildDockSpace(child_a, node->FirstChild.get());
		//		BuildDockSpace(child_b, node->SecChild.get());

		//		return parentID;
		//	}
		//}

		//void RestoreLayout(FrameNode* root)
		//{
		//	ImGuiID dockspace_id = ImGui::GetID(mainDockSpaceLabel.c_str());
		//	ImGui::DockBuilderRemoveNode(dockspace_id);
		//	ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		//	ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

		//	BuildDockSpace(dockspace_id, root);

		//	ImGui::DockBuilderFinish(dockspace_id);
		//}

		//template<typename T>
	//T* GetPanel()
	//{
	//	static_assert(std::is_base_of<EditorPanel, T>::value, "T must be derived from EditorPanel");
	//	for (auto& panel : m_panels)
	//		if (T* castedPanel = dynamic_cast<T*>(panel.get()))
	//			return castedPanel;
	//	return nullptr;
	//}