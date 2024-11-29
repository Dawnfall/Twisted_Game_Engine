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

		std::vector<CTransform*> rootTransforms = CTransform::GetRootTransforms(*editor->GetActiveWorld());

		std::function<void(const CTransform*)> renderTreeObject;
		renderTreeObject = [&editor, &renderTreeObject](const CTransform* transform)
			{
				if (ImGui::TreeNodeEx((transform->GetName() + std::to_string((int)transform->GetEntityID())).c_str()))
				{
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
}