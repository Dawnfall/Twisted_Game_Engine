#include "TreeViewPanel.h"
#include "EditorLayer.h"

#include "Twisted/Gameing/World.h"
#include "Twisted/Gameing/Entity.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/CName.h"

#include "EditorRegistry.h"
#include "UI/ImguiExtensions.h"

namespace Twisted::Editor
{
	static ImGuiTreeNodeFlags GetNodeFlags(size_t childCount, Entity entity)
	{
		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_DefaultOpen |
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		if (childCount == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (EditorLayer::GetInstance()->GetSelection().GetSelectedEntities().contains(entity))
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		return flags;
	}

	static void CheckLeftClickOnNode(TreeViewToken& token, Entity entity)
	{
		if (EditorLayer::GetInstance()->GetInput().IsClicked() && ImGui::IsItemHovered())//!ImGui::IsItemToggledOpen()
		{
			ImGuiIO& io = ImGui::GetIO();
			bool ctrlHeld = io.KeyCtrl; // true if Ctrl is held
			bool shiftHeld = io.KeyShift; // true if Shift is held
			bool altHeld = io.KeyAlt; // true if Alt is held

			if (ctrlHeld)
			{
				EditorLayer::GetInstance()->GetSelection().SelectEntities({ entity }, SelectionFlags::REMOVE_IF_SELECTED);
			}
			else if (shiftHeld)
			{
				//TODO:should select all visible from prev tto current
			}
			else
			{
				EditorLayer::GetInstance()->GetSelection().SelectEntities({ entity }, SelectionFlags::REMOVE_OTHERS);
			}
		}
	}

	static void CheckRightClickOnNode(Entity entity, TreeViewToken& token)
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

	static void CheckRightClickOnEmpty(TreeViewToken& token)
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

	void TreeViewPanel::PaintContent()
	{
		auto editor = EditorLayer::GetInstance();
		auto gameWorld = editor->GetGameWorld();

		if (!EditorLayer::GetInstance()->GetGameWorld())
			return;

		//SetNodeBackgroundColor(); //TODO:... must be done probably somewhere else ??!? not sure

		TreeViewToken token;
		token.EntireRegion = ImGui::GetContentRegionAvail();

		auto& rootManager = EditorLayer::GetInstance()->GetGameWorld()->ForceGetManager<RootTransformManager>();
		for (auto rootEnt : rootManager.RootEntities)
			RenderTreeNode(Entity{ rootEnt,EditorLayer::GetInstance()->GetGameWorld()}, token);

		CheckRightClickOnEmpty(token);
		HandleChanges(token);
	}

	void TreeViewPanel::RenderTreeNode(Entity entity, TreeViewToken& token)
	{
		CTransform& transform = entity.GetWorld()->GetComponent<CTransform>(entity.GetID());
		const CName& name = entity.GetWorld()->GetComponent<CName>(entity.GetID());

		std::string nodeID = name.Name + "###" + std::to_string((uint64_t)entity.GetID());

		ImGuiTreeNodeFlags flags = GetNodeFlags(transform.GetChildCount(), entity);

		bool IsSelected = EditorLayer::GetInstance()->GetSelection().GetSelectedEntities().contains(entity);
		bool isOpened = ImGui::TreeNodeEx(nodeID.c_str(), flags);
		bool isHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly);

		// drag soruce
		Im::DragSource<Entity>(Constants::ENTITY_DRAG_TYPE, entity, name.Name.c_str());

		//drag target
		auto dropResult = Im::DragTarget<Entity>(Constants::ENTITY_DRAG_TYPE, Entity::Invalid());
		if (dropResult.first)
		{
			token.NewEntityParent = entity;
			token.NewIndex = transform.GetChildCount();
			token.DraggedEntity = dropResult.second;
			token.IsDropped = true;
		}

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

		auto dragResult = Im::DragTarget(Constants::ENTITY_DRAG_TYPE, Entity::Invalid());
		if (dragResult.first)
		{
			token.NewIndex = transform.GetSiblingsIndex();
			if (isAfter)
				token.NewIndex += 1;
			token.NewEntityParent = entity;
			token.DraggedEntity = dragResult.second;
			token.IsDropped = true;
		}
	}

	void TreeViewPanel::HandleChanges(TreeViewToken& token)
	{
		if (token.IsDropped)
		{
			CTransform* newParentTransform = token.NewEntityParent.GetWorld()->TryGetComponent<CTransform>(token.NewEntityParent.GetID());
			CTransform* draggedTransform = token.DraggedEntity.GetWorld()->TryGetComponent<CTransform>(token.DraggedEntity.GetID());

			if (!draggedTransform->IsDescendant(newParentTransform))
			{
				CTransform::SetParent(*draggedTransform, newParentTransform);

				size_t currIndex = draggedTransform->GetSiblingsIndex();

				if (token.NewIndex > currIndex)
					token.NewIndex -= 1;

				CTransform::SetSiblingsIndex(*draggedTransform, token.NewIndex);
			}
		}

		if (token.EntityToDelete)
		{
			Entity entToDelete = token.EntityToDelete;
			token.EntityToDelete.GetWorld()->DestroyEntity(entToDelete.GetID());
			EditorLayer::GetInstance()->GetSelection().ClearEntities();
		}

		if (token.doCreateNew)
		{
			Entity newEntity = EditorLayer::GetInstance()->GetGameWorld()->CreateNewEntity();
			if (token.NewEntityParent)
			{
				CTransform& transform = newEntity.GetWorld()->GetComponent<CTransform>(newEntity.GetID());
				CTransform* newEntityParent = token.NewEntityParent.GetWorld()->TryGetComponent<CTransform>(token.NewEntityParent.GetID());
				CTransform::SetParent(transform, newEntityParent);
			}

		}
	}
}

REGISTER_EDITOR_PANEL(TreeViewPanel)
