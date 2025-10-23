#include "TreeViewPanel.h"
#include "Twisted/Gameing/World.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/CName.h"
#include "EditorLayer.h"

#include "Twisted/Gameing/Components/CRenderer.h"
#include "Twisted/Gameing/Entity.h"

#include "EditorMacros.h"
#include "EditorRegistry.h"
#include "EditorData/EditorData.h"

namespace Twisted::Editor
{
	void TreeViewPanel::PaintContent()
	{
		if (!EditorData::GetInstance().GetGameWorld())
			return;

		//SetNodeBackgroundColor(); //TODO:... must be done probably somewhere else ??!? not sure

		TreeViewToken token;
		token.EntireRegion = ImGui::GetContentRegionAvail();

		auto& rootEntities = EditorData::GetInstance().GetGameWorld()->GetRootEntities();
		if (rootEntities.size() > 0)
			RenderDropZone(Entity{ rootEntities[0],EditorData::GetInstance().GetGameWorld() }, token, false);
		for (int i = 0; i < rootEntities.size(); i++)
			RenderTreeNode(Entity{ rootEntities[i],EditorData::GetInstance().GetGameWorld() }, token);

		CheckRightClickOnEmpty(token);
		HandleChanges(token);
	}

	void TreeViewPanel::RenderTreeNode(Entity entity, TreeViewToken& token)
	{
		CTransform& transform = entity.GetWorld()->GetComponent<CTransform>(entity.GetID());
		const CName& name = entity.GetWorld()->GetComponent<CName>(entity.GetID());

		std::string nodeID = name.GetName() + "###" + std::to_string((uint64_t)entity.GetID());

		ImGuiTreeNodeFlags flags = GetNodeFlags(transform.GetChildCount(), entity);

		bool IsSelected = EditorData::GetInstance().GetSelection().GetSelectedEntities().contains(entity);
		bool isOpened = ImGui::TreeNodeEx(nodeID.c_str(), flags);
		bool isHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly);

		DragDrop(entity, transform.GetChildCount(), name.GetName());
		CheckRightClickOnNode(entity, token);
		CheckLeftClickOnNode(token, entity);

		if (IsSelected && ImGui::IsKeyPressed(ImGuiKey_Delete))
			token.EntityToDelete = entity;

		auto& children = transform.GetChildrenIDs();
		if (isOpened)
		{
			for (size_t i = 0; i < children.size(); i++)
			{
				if (i == 0)
					RenderDropZone(Entity{ children[i],entity.GetWorld() }, token, false);
				RenderTreeNode(Entity{ children[i],entity.GetWorld() }, token);
			}
			ImGui::TreePop();
		}

		if (!isOpened || transform.GetChildCount() == 0)
		{
			bool isLast = transform.GetSiblingsIndex() == (transform.GetSiblingsCount() - 1);
			if (isLast) //recursive set drop for all parents where child is last
			{
				RenderDropZone(entity, token, true);
				//TODO:...		
			}
			else //set only for this
			{
				RenderDropZone(entity, token, true);
			}
		}
	}
	void TreeViewPanel::RenderDropZone(Entity entity, TreeViewToken& token, bool isAfter)
	{
		CTransform& transform = entity.GetWorld()->GetComponent<CTransform>(entity.GetID());

		// Insert a dummy drop zone BEFORE the node for unparenting
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		ImVec2 dropZoneSize(ImGui::GetContentRegionAvail().x, 4.0f);

		std::string id = (isAfter) ? "##drop_zone_after_" : "##drop_zone_before_";
		id += std::to_string(static_cast<int>(entity.GetID()));
		ImGui::InvisibleButton(id.c_str(), dropZoneSize);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DRAG_TREE_TRANSFORM.c_str()))
			{
				m_newIndex = transform.GetSiblingsIndex();
				if (isAfter)
					m_newIndex += 1;
				newParentEntity = entity;
				draggedEntity = *static_cast<Entity*>(payload->Data);
				m_isDroped = true;
				//std::string output = std::format("On drag finish\n isAfter = {}\n dropEnt = {}\n draggedEnt = {}\n", m_isAfter, (int)m_dropedOnEnt, (int)m_draggedEnt);
				//TWISTED_INFO(output);
			}
			ImGui::EndDragDropTarget();
		}
	}


	void TreeViewPanel::DragDrop(Entity entity, size_t childCount, const std::string& name)
	{
		// Drag source
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(DRAG_TREE_TRANSFORM.c_str(), &entity, sizeof(Entity));
			ImGui::Text("Dragging %s", name.c_str());
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DRAG_TREE_TRANSFORM.c_str()))
			{
				m_newIndex = childCount;
				draggedEntity = *static_cast<Entity*>(payload->Data);
				newParentEntity = entity;
				m_isDroped = true;
			}
			ImGui::EndDragDropTarget();
		}
	}

	void TreeViewPanel::CheckLeftClickOnNode(TreeViewToken& token, Entity entity)
	{
		bool isLeftClicked = EditorData::GetInstance().GetInput().IsClicked();

		if (isLeftClicked && ImGui::IsItemHovered())//!ImGui::IsItemToggledOpen()
		{
			ImGuiIO& io = ImGui::GetIO();
			bool ctrlHeld = io.KeyCtrl; // true if Ctrl is held
			bool shiftHeld = io.KeyShift; // true if Shift is held
			bool altHeld = io.KeyAlt; // true if Alt is held

			if (ctrlHeld)
			{
				EditorData::GetInstance().GetSelection().SelectEntities({ entity }, SelectionFlags::REMOVE_IF_SELECTED);
			}
			else if (shiftHeld) 
			{
				//TODO:should select all visible from prev tto current
			}
			else
			{
				EditorData::GetInstance().GetSelection().SelectEntities({ entity }, SelectionFlags::REMOVE_OTHERS);
			}
		}
	}

	void TreeViewPanel::CheckRightClickOnNode(Entity entity, TreeViewToken& token)
	{
		if (!token.IsClickUsed && ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup(nodeContextPopup.c_str());
			token.IsClickUsed = true;
		}

		if (ImGui::BeginPopup(nodeContextPopup.c_str()))
		{
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Empty"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = entity;
				}
				if (ImGui::MenuItem("Sphere"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = entity;
				}
				if (ImGui::MenuItem("Cube"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = entity;
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Delete"))
			{
				token.EntityToDelete = entity;
			}
			ImGui::EndPopup();
		}
	}

	void TreeViewPanel::CheckRightClickOnEmpty(TreeViewToken& token)
	{
		ImGui::InvisibleButton("EmptyTreePanel", token.EntireRegion);
		if (!token.IsClickUsed && ImGui::IsItemClicked(ImGuiMouseButton_Right))
			ImGui::OpenPopup(emptyContextPopup.c_str());

		if (ImGui::BeginPopup(emptyContextPopup.c_str()))
		{
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Empty"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = Entity::Invalid();
				}
				if (ImGui::MenuItem("Sphere"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = Entity::Invalid();
				}
				if (ImGui::MenuItem("Cube"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = Entity::Invalid();
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}
	}

	void TreeViewPanel::HandleChanges(TreeViewToken& token)
	{
		World* world = EditorData::GetInstance().GetGameWorld();

		if (m_isDroped)
		{
			CTransform* newParentTransform = newParentEntity.GetWorld()->TryGetComponent<CTransform>(newParentEntity.GetID());
			CTransform* draggedTransform = draggedEntity.GetWorld()->TryGetComponent<CTransform>(draggedEntity.GetID());

			if (!draggedTransform->IsDescendant(newParentTransform))
			{
				draggedTransform->SetParent(newParentTransform);

				size_t currIndex = draggedTransform->GetSiblingsIndex();

				if (m_newIndex > currIndex)
					m_newIndex -= 1;

				draggedTransform->SetSiblingsIndex(m_newIndex);
			}

			newParentEntity = Entity::Invalid();
			draggedEntity = Entity::Invalid();
			m_newIndex = 0;
			m_isDroped = false;
		}

		if (token.EntityToDelete)
			token.EntityToDelete.GetWorld()->DestroyEntity(token.EntityToDelete.GetID());

		if (token.doCreateNew)
		{
			Entity newEntity = world->CreateNew();
			newEntity.GetWorld()->GetComponent<CName>(newEntity.GetID()).SetName("New Entity");

			if (token.NewEntityParent)
				newEntity.GetWorld()->GetComponent<CTransform>(newEntity.GetID()).SetParent(token.NewEntityParent.GetWorld()->TryGetComponent<CTransform>(token.NewEntityParent.GetID()));

		}
	}
}

REGISTER_EDITOR_PANEL(TreeViewPanel)