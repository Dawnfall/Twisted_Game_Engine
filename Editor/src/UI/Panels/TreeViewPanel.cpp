#include "editorpch.h"

#include "TreeViewPanel.h"
#include "EditorApp.h"
#include "Twisted/Game/World.h"
#include "Twisted/Game/Components/CTransform.h"

namespace Twisted::Editor
{
	void TreeViewPanel::RenderContent(EditorApp* editor)
	{
		auto world = editor->GetActiveWorld();
		if (!world)
			return;

		std::vector<CTransform*> rootTransforms = CTransform::GetRootTransforms();

		std::function<void(const CTransform*)> renderTreeObject;
		renderTreeObject = [&editor, &renderTreeObject](const CTransform* transform)
			{
				bool isSelected = (editor->GetSelectedEntity() == transform->GetEntityID());
				ImGuiTreeNodeFlags flags = SetTreeFlags(*transform, editor);
				std::string nodeID = transform->GetName() + std::to_string((int)transform->GetEntityID());
				// Drag source
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("drag_tree_transform", &transform, sizeof(CTransform*));
					ImGui::Text("Dragging %s", transform->GetName().c_str());
					ImGui::EndDragDropSource();
				}
				// Drop target
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("drag_tree_transform"))
					{
						CTransform* droppedTransform = *(CTransform**)payload->Data;
						// Handle the drop action here, e.g., reparenting the droppedTransform
					}
					ImGui::EndDragDropTarget();
				}
				if (ImGui::TreeNodeEx(nodeID.c_str(),flags))
				{
					if (ImGui::IsItemClicked())
					{
						editor->SetSelectedEntity(transform->GetEntityID());
					}
					for (EntityID child : transform->GetChildren())
					{
						renderTreeObject(editor->GetActiveWorld()->GetComponent<CTransform>(child));
					}
					ImGui::TreePop();
				}
			};

		for (const CTransform* root : rootTransforms)
		{
			renderTreeObject(root);
		}
	}

	ImGuiTreeNodeFlags TreeViewPanel::SetTreeFlags(const CTransform& transform, EditorApp* app)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		if (transform.GetChildCount() == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;
		if (app->GetSelectedEntity() == transform.GetEntityID())
			flags |= ImGuiTreeNodeFlags_Selected;
		return flags;
	}
}