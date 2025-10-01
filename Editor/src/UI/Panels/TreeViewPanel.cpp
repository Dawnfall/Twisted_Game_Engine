#include "TreeViewPanel.h"
#include "Twisted/Gameing/World.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/CName.h"
#include "EditorLayer.h"
#include "Selection.h"

#include "Twisted/Gameing/Components/CRenderer.h"
namespace Twisted::Editor
{
	void TreeViewPanel::PaintContent()
	{
		//if (!m_editor->GetGameWorld())
		//	return;

		//SetNodeBackgroundColor(); //TODO:... must be done probably somewhere else ??!? not sure

		//TreeViewToken token;
		//token.EntireRegion = ImGui::GetContentRegionAvail();

		//int selected_id = -1;
		//bool isLeftClick = false;
		//bool isRightClick = false;

		//auto& rootEntities = m_editor->GetGameWorld()->GetRootEntities();
		//for (int i = 0; i < rootEntities.size(); i++)
		//{
		//	if (i == 0)
		//		RenderDropZone(rootEntities[i], token, false);
		//	RenderTreeNode(rootEntities[i], token);
		//}

		//RightClickEmpty(token);
		//HandleChanges(token);
	}

	void TreeViewPanel::RenderTreeNode(EntityID entity, TreeViewToken& token)
	{
		const CTransform& transform = m_editor->GetGameWorld()->GetComponent<CTransform>(entity);
		const CName& name = m_editor->GetGameWorld()->GetComponent<CName>(entity);

		std::string nodeID = name.GetName() + "###" + std::to_string((int)name.GetEntityID());

		bool isSelected = Selection::GetInstance().Contains(entity);


		ImGuiTreeNodeFlags flags = GetNodeFlags(transform.GetChildCount(), isSelected);

		bool isOpened = ImGui::TreeNodeEx(nodeID.c_str(), flags);
		bool isHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly);


		DragDrop(entity, transform.GetChildCount(), name.GetName());
		RightClickOnNode(entity, token);
		LeftClickOnNode(entity, isSelected);

		if (isSelected && ImGui::IsKeyPressed(ImGuiKey_Delete))
			token.EntityToDelete = entity;

		if (isOpened)
		{
			for (size_t i = 0; i < transform.GetChildrenIDs().size(); i++)
			{
				if (i == 0)
					RenderDropZone(transform.GetChildrenIDs()[i], token, false);
				RenderTreeNode(transform.GetChildrenIDs()[i], token);
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
	void TreeViewPanel::RenderDropZone(EntityID entity, TreeViewToken& token, bool isAfter)
	{
		const CTransform& transform = m_editor->GetGameWorld()->GetComponent<CTransform>(entity);

		// Insert a dummy drop zone BEFORE the node for unparenting
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		ImVec2 dropZoneSize(ImGui::GetContentRegionAvail().x, 4.0f);

		std::string id = (isAfter) ? "##drop_zone_after_" : "##drop_zone_before_";
		id += std::to_string(static_cast<int>(entity));
		ImGui::InvisibleButton(id.c_str(), dropZoneSize);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DRAG_TREE_TRANSFORM.c_str()))
			{
				m_newIndex = transform.GetSiblingsIndex();
				if (isAfter)
					m_newIndex += 1;
				m_newParentID = transform.GetParentID();
				m_draggedID = *static_cast<EntityID*>(payload->Data);
				m_isDroped = true;
				//std::string output = std::format("On drag finish\n isAfter = {}\n dropEnt = {}\n draggedEnt = {}\n", m_isAfter, (int)m_dropedOnEnt, (int)m_draggedEnt);
				//TWISTED_INFO(output);
			}
			ImGui::EndDragDropTarget();
		}
	}


	void TreeViewPanel::DragDrop(EntityID entID, size_t childCount, const std::string& name)
	{
		// Drag source
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(DRAG_TREE_TRANSFORM.c_str(), &entID, sizeof(EntityID));
			ImGui::Text("Dragging %s", name.c_str());
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DRAG_TREE_TRANSFORM.c_str()))
			{
				m_newIndex = childCount;
				m_draggedID = *static_cast<EntityID*>(payload->Data);
				m_newParentID = entID;
				m_isDroped = true;
			}
			ImGui::EndDragDropTarget();
		}
	}

	void TreeViewPanel::LeftClickOnNode(EntityID entID, bool isSelected)
	{
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			ImGuiIO& io = ImGui::GetIO();
			bool ctrlHeld = io.KeyCtrl; // true if Ctrl is held
			bool shiftHeld = io.KeyShift; // true if Shift is held
			bool altHeld = io.KeyAlt; // true if Alt is held

			if (ctrlHeld)
			{
				if (isSelected)
					Selection::GetInstance().DeSelect(entID);
				else
					Selection::GetInstance().MultiSelect<EntityID>({entID});
			}
			else if (shiftHeld) //TODO:should select all visible from prev tto current
			{

			}
			else
			{
				Selection::GetInstance().SingleSelect<EntityID>(entID);
			}
		}
	}

	void TreeViewPanel::RightClickOnNode(EntityID entity, TreeViewToken& token)
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

	void TreeViewPanel::RightClickEmpty(TreeViewToken& token)
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
					token.NewEntityParent = NullEntity;
				}
				if (ImGui::MenuItem("Sphere"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = NullEntity;
				}
				if (ImGui::MenuItem("Cube"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = NullEntity;
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}
	}

	void TreeViewPanel::HandleChanges(TreeViewToken& token)
	{
		World* world = m_editor->GetGameWorld();

		if (m_isDroped)
		{
			CTransform* newParentTransform = world->TryGetComponent<CTransform>(m_newParentID);
			CTransform* draggedTransform = world->TryGetComponent<CTransform>(m_draggedID);

			if (!draggedTransform->IsDescendant(newParentTransform))
			{
				draggedTransform->SetParent(newParentTransform);

				size_t currIndex = draggedTransform->GetSiblingsIndex();

				if (m_newIndex > currIndex)
					m_newIndex -= 1;

				draggedTransform->SetSiblingsIndex(m_newIndex);
			}

			m_newParentID = NullEntity;
			m_draggedID = NullEntity;
			m_newIndex = 0;
			m_isDroped = false;
		}

		if (token.EntityToDelete != NullEntity)
			world->DestroyEntity(token.EntityToDelete);

		if (token.doCreateNew)
		{
			EntityID newEntity = world->CreateEntity();
			world->GetComponent<CName>(newEntity).SetName("New Entity");

			if (token.NewEntityParent != NullEntity);
			world->GetComponent<CTransform>(newEntity).SetParent(world->TryGetComponent<CTransform>(token.NewEntityParent));

		}
	}
}