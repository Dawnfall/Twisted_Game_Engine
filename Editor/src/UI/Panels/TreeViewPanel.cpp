#include "editorpch.h"

#include "TreeViewPanel.h"
#include "EditorLayer.h"
//#include "Twisted/Game/World.h"
#include "Twisted/Game/Components/CTransform.h"
#include "Twisted/Game/Components/CName.h"

namespace Twisted::Editor
{
	void TreeViewPanel::RenderContent()
	{
		auto world = editor->GetActiveWorld();
		if (!world)
			return;

		RenderTreeNode(NullEntity);

		//handle change
		if (m_siblingIndex >= 0)
		{
			CTransform& draggedTransform = world->GetComponent<CTransform>(m_draggedEnt);
			draggedTransform.SetParent(m_dropedOnEnt);
			draggedTransform.SetSiblingsIndex(m_siblingIndex);

			m_draggedEnt = NullEntity;
			m_dropedOnEnt = NullEntity;
			m_siblingIndex = -1;

		}
	}

	void TreeViewPanel::RenderTreeNode(EntityID entity)
	{
		if (entity == NullEntity)
		{
			int rootsCount = editor->GetActiveWorld()->GetRootEntities().size();
			for (int i = 0; i < rootsCount; i++)
			{
				RenderDropZone(NullEntity, i,false);
				RenderTreeNode(editor->GetActiveWorld()->GetRootEntities()[i]);
				if (i == rootsCount - 1)
					RenderDropZone(NullEntity, i + 1,true);
			}
		}
		else
		{
			CTransform& transform = editor->GetActiveWorld()->GetComponent<CTransform>(entity);
			const CName& name = editor->GetActiveWorld()->GetComponent<CName>(entity);

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
			if (transform.GetChildCount() == 0)
				flags |= ImGuiTreeNodeFlags_Leaf;
			if (editor->GetSelectedEntity() == transform.GetEntityID())
				flags |= ImGuiTreeNodeFlags_Selected;

			std::string nodeID = name.GetName() + "###" + std::to_string((int)entity);

			if (ImGui::TreeNodeEx(nodeID.c_str(), flags))
			{
				// Drag source
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("drag_tree_transform", &entity, sizeof(CTransform*));
					ImGui::Text("Dragging %s", name.GetName().c_str());
					ImGui::EndDragDropSource();
				}
				if (ImGui::IsItemClicked())
				{
					editor->SetSelectedEntity(entity);
				}

				RenderDropZone(entity, transform.GetSiblingsIndex(), false);
				for (EntityID child : transform.GetChildren())
					RenderTreeNode(child);
				if (transform.GetSiblingsIndex() == transform.GetSiblingsCount() - 1)
					RenderDropZone(entity, transform.GetSiblingsCount()+1,true);

				ImGui::TreePop();
			}
		}
	}

	void TreeViewPanel::RenderDropZone(EntityID entity, unsigned int position,bool isAfter)
	{
		// Insert a dummy drop zone BEFORE the node for unparenting
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		ImVec2 dropZoneSize(ImGui::GetContentRegionAvail().x, 4.0f);

		std::string idPart1 = (isAfter) ? "##drop_zone_after_" : "##drop_zone_before_";

		ImGui::InvisibleButton((idPart1 + std::to_string((int)entity)).c_str(), dropZoneSize);

		// Check for drop before the node
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("drag_tree_transform"))
			{
				m_dropedOnEnt = entity;
				m_siblingIndex = position;
				m_draggedEnt = *static_cast<EntityID*>(payload->Data);
			}
			ImGui::EndDragDropTarget();
		}
	}
}