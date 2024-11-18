#include "editorpch.h"

#include "TreeViewPanel.h"
#include "EditorApp.h"
#include "Twisted/Game/World.h"
#include "Twisted/Game/Components/CTransform.h"

namespace Twisted::Editor
{
	void TreeViewPanel::RenderContent(EditorApp* editor)
	{
		auto world = editor->GetWorld();
		std::vector<CTransform*> rootTransforms = CTransform::GetRootTransforms(*editor->GetWorld());

		std::function<void(const CTransform*)> renderTreeObject;
		renderTreeObject = [&world, &renderTreeObject](const CTransform* transform)
			{
				if (ImGui::TreeNodeEx((transform->GetName() + std::to_string((int)transform->GetEntityID())).c_str()))
				{
					for (EntityID child : transform->GetChildren())
					{
						renderTreeObject(world->GetComponent<CTransform>(child));
					}
					ImGui::TreePop();
				}
			};

		for (const CTransform* root : rootTransforms)
		{
			renderTreeObject(root);
		}
	}
}