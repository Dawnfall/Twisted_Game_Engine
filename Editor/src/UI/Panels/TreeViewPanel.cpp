#include "TreeViewPanel.h"
//#include "Twisted/Game/World.h"
#include "Game/Components/CTransform.h"
#include "Game/Components/CName.h"
#include "EditorRuntime.h"

namespace Twisted::Editor
{
	void TreeViewPanel::RenderContent()
	{
		if (!m_editor->GetGameWorld())
			return;

		RenderTreeNode(NullEntity);

		//handle change
		if (m_siblingIndex >= 0)
		{
			CTransform& draggedTransform = m_editor->GetGameWorld()->GetComponent<CTransform>(m_draggedEnt);
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
			int rootsCount = m_editor->GetGameWorld()->GetRootEntities().size();
			for (int i = 0; i < rootsCount; i++)
			{
				RenderDropZone(NullEntity, i, false);
				RenderTreeNode(m_editor->GetGameWorld()->GetRootEntities()[i]);
				if (i == rootsCount - 1)
					RenderDropZone(NullEntity, i + 1, true);
			}
		}
		else
		{
			CTransform& transform = m_editor->GetGameWorld()->GetComponent<CTransform>(entity);
			const CName& name = m_editor->GetGameWorld()->GetComponent<CName>(entity);

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
			if (transform.GetChildCount() == 0)
				flags |= ImGuiTreeNodeFlags_Leaf;
			if (m_editor->GetSelectedEntity() == transform.GetEntityID())
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
					m_editor->SetSelectedEntity(entity);
				}

				RenderDropZone(entity, transform.GetSiblingsIndex(), false);
				for (EntityID child : transform.GetChildren())
					RenderTreeNode(child);
				if (transform.GetSiblingsIndex() == transform.GetSiblingsCount() - 1)
					RenderDropZone(entity, transform.GetSiblingsCount() + 1, true);

				ImGui::TreePop();
			}
		}
	}

	void TreeViewPanel::RenderDropZone(EntityID entity, unsigned int position, bool isAfter)
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