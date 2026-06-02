#include "UI/Panels/TreeViewPanel.h"

#include "EditorApp/EditorService.h"
#include "Input.h"
#include "World.h"
#include "Entity.h"
#include "Components/CTransform.h"
#include "Components/CName.h"

#include "EditorApp/EditorRegistry.h"
#include "UI/ImguiExtensions.h"
#include "SceneInstantiator.h"

#include "GameService.h"
#include "EditorData/Selection.h"
#include "EditorConstants.h"
#include "Managers/RootTransformManager.h"

#include <imgui.h>
#include <string>
#include <cstdint>

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

		if (Application::GetInstance().GetService<EditorService>()->GetSelection().GetSelectedEntities().contains(entity))
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		return flags;
	}

	static void CheckLeftClickOnNode(TreeViewToken& token, Entity entity)
	{
		(void)token;
		if (Input::GetInstance().IsClicked() && ImGui::IsItemHovered())//!ImGui::IsItemToggledOpen()
		{
			ImGuiIO& io = ImGui::GetIO();
			bool ctrlHeld = io.KeyCtrl; // true if Ctrl is held
			bool shiftHeld = io.KeyShift; // true if Shift is held
			//bool altHeld = io.KeyAlt; // true if Alt is held

			if (ctrlHeld)
			{
				Application::GetInstance().GetService<EditorService>()->GetSelection().SelectEntities({ entity }, SelectionFlags::REMOVE_IF_SELECTED);
			}
			else if (shiftHeld)
			{
				//TODO:should select all visible from prev tto current
			}
			else
			{
				Application::GetInstance().GetService<EditorService>()->GetSelection().SelectEntities({ entity }, SelectionFlags::REMOVE_OTHERS);
			}
		}
	}

	static void CheckRightClickOnNode(Entity entity, TreeViewToken& token, Entity& contextMenuEntity)
	{
		if (!token.IsClickUsed && ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup(nodeContextPopup.c_str());
			token.IsClickUsed = true;
			contextMenuEntity = entity;
		}

		if (ImGui::BeginPopup(nodeContextPopup.c_str()))
		{
			Entity contextEntity = contextMenuEntity;
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Empty"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = contextEntity;
				}
				if (ImGui::MenuItem("Sphere"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = contextEntity;
				}
				if (ImGui::MenuItem("Cube"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = contextEntity;
				}
				if (ImGui::MenuItem("Quad"))
				{
					token.doCreateNew = true;
					token.NewEntityParent = contextEntity;
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Delete"))
			{
				token.EntitiesToDelete.push_back(contextEntity);
			}
			ImGui::EndPopup();
		}
	}

	static void CheckRightClickOnEmpty(TreeViewToken& token)
	{
		ImGui::InvisibleButton("EmptyTreePanel", token.EntireRegion);

		// asset drop on empty space → instantiate at root
		auto assetDrop = Im::DragTarget<ImportedAsset*>(Constants::ASSET_DRAG_TYPE, nullptr);
		if (assetDrop.first && assetDrop.second)
		{
			token.DroppedAsset = assetDrop.second;
			token.AssetDropParent = Entity::Invalid();
		}

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
				if (ImGui::MenuItem("Quad"))
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
		//auto editor = EditorWorldService::GetInstance();
		TreeViewToken token;
		token.gameWorld = Application::GetInstance().GetService<GameService>()->GetGameWorld();

		if (!token.gameWorld)
			return;

		//SetNodeBackgroundColor(); //TODO:... must be done probably somewhere else ??!? not sure

		token.EntireRegion = ImGui::GetContentRegionAvail();

		auto& rootManager = token.gameWorld->ForceGetManager<RootTransformManager>();
		for (auto rootEnt : rootManager.RootEntities)
			RenderTreeNode(Entity{ rootEnt,token.gameWorld }, token);

		CheckRightClickOnEmpty(token);
		HandleChanges(token);
	}

	void TreeViewPanel::RenderTreeNode(Entity entity, TreeViewToken& token)
	{
		TransformComponent& transform = entity.GetWorld()->GetComponent<TransformComponent>(entity.GetID());
		const NameComponent& name = entity.GetWorld()->GetComponent<NameComponent>(entity.GetID());

		std::string nodeID = name.Name + "###" + std::to_string((uint64_t)entity.GetID());

		ImGuiTreeNodeFlags flags = GetNodeFlags(transform.GetChildCount(), entity);

		bool IsSelected = Application::GetInstance().GetService<EditorService>()->GetSelection().GetSelectedEntities().contains(entity);
		bool isOpened = ImGui::TreeNodeEx(nodeID.c_str(), flags);
		//bool isHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly);

		// drag soruce
		Im::DragSource<Entity>(Constants::ENTITY_DRAG_TYPE, entity, name.Name.c_str());

		//drag target — entity reorder
		auto dropResult = Im::DragTarget<Entity>(Constants::ENTITY_DRAG_TYPE, Entity::Invalid());
		if (dropResult.first)
		{
			token.NewEntityParent = entity;
			token.NewIndex = transform.GetChildCount();
			token.DraggedEntity = dropResult.second;
			token.IsDropped = true;
		}

		// drag target — asset instantiate as child
		auto assetDrop = Im::DragTarget<ImportedAsset*>(Constants::ASSET_DRAG_TYPE, nullptr);
		if (assetDrop.first && assetDrop.second)
		{
			token.DroppedAsset = assetDrop.second;
			token.AssetDropParent = entity;
		}

		CheckRightClickOnNode(entity, token, m_contextMenuEntity);
		CheckLeftClickOnNode(token, entity);

		if (IsSelected && ImGui::IsKeyPressed(ImGuiKey_Delete))
			token.EntitiesToDelete.push_back(entity);

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
		TransformComponent& transform = entity.GetWorld()->GetComponent<TransformComponent>(entity.GetID());

		// Insert a dummy drop zone BEFORE the node for unparenting
		[[maybe_unused]] ImVec2 cursorPos = ImGui::GetCursorScreenPos();
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
			TransformComponent* newParentTransform = token.NewEntityParent.GetWorld()->TryGetComponent<TransformComponent>(token.NewEntityParent.GetID());
			TransformComponent* draggedTransform = token.DraggedEntity.GetWorld()->TryGetComponent<TransformComponent>(token.DraggedEntity.GetID());

			if (!draggedTransform->IsDescendant(newParentTransform))
			{
				TransformComponent::SetParent(*draggedTransform, newParentTransform);

				size_t currIndex = draggedTransform->GetSiblingsIndex();

				if (token.NewIndex > currIndex)
					token.NewIndex -= 1;

				TransformComponent::SetSiblingsIndex(*draggedTransform, token.NewIndex);
			}
		}

		if (!token.EntitiesToDelete.empty())
		{
			for (Entity& entToDelete : token.EntitiesToDelete)
				entToDelete.GetWorld()->DestroyEntity(entToDelete.GetID());
			Application::GetInstance().GetService<EditorService>()->GetSelection().ClearEntities();
		}

		if (token.doCreateNew)
		{
			Entity newEntity = Application::GetInstance().GetService<GameService>()->GetGameWorld()->CreateNewEntity();
			if (token.NewEntityParent)
			{
				TransformComponent& transform = newEntity.GetWorld()->GetComponent<TransformComponent>(newEntity.GetID());
				TransformComponent* newEntityParent = token.NewEntityParent.GetWorld()->TryGetComponent<TransformComponent>(token.NewEntityParent.GetID());
				TransformComponent::SetParent(transform, newEntityParent);
			}
		}

		if (token.DroppedAsset)
		{
			World* world = Application::GetInstance().GetService<GameService>()->GetGameWorld();
			if (world)
			{
				TransformComponent* parentTransform = nullptr;
				if (token.AssetDropParent)
					parentTransform = token.AssetDropParent.GetWorld()->TryGetComponent<TransformComponent>(token.AssetDropParent.GetID());
				InstantiateAsset(token.DroppedAsset, world, parentTransform);
			}
		}
	}
}

REGISTER_EDITOR_PANEL(TreeViewPanel)
