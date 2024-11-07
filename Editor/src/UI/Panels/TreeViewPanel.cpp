#include "editorpch.h"

#include "TreeViewPanel.h"
#include "EditorApp.h"
#include "Twisted/Game/EcsManager.h"
#include "Twisted/Game/Components/CTransform.h"

namespace Twisted::Editor
{
	void TreeViewPanel::RenderContent(EditorApp* editor)
	{
		EcsManager& ecs = editor->GetWorld()->GetEcs();
		std::vector<CTransform*> rootTransforms= editor->GetWorld()->GetRootTransforms();

		std::function<void(const CTransform*)> renderTreeObject;
		renderTreeObject = [&ecs, &renderTreeObject](const CTransform* transform)
			{
				if (ImGui::TreeNodeEx((transform->GetName()+std::to_string((int)transform->GetID())).c_str()))
				{
					for (entt::entity childID : transform->GetChildIDs())
					{
						renderTreeObject(ecs.GetComponent<CTransform>(childID));
					}
					ImGui::TreePop();
				}
			};

		for (const CTransform* root : rootTransforms)
		{
			renderTreeObject(root);
		}
	}

	//void TreeWindow::RenderContent(Twisted::Application* app)
	//{
	//	return;
	//}
}