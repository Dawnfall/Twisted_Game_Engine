#include "pch.h"
#include "TreeWindow.h"
#include "Twisted/Application.h"
#include "Twisted/Game/EcsManager.h"

namespace Twisted::Editor
{
	void TreeWindow::RenderContent(Application* app)
	{
		EcsManager& ecs = app->Game.Ecs;
		std::vector<CTransform*> rootTransforms= app->Game.GetRootTransforms();

		std::function<void(const CTransform*)> renderTreeObject;
		renderTreeObject = [&ecs, &renderTreeObject](const CTransform* transform)
			{
				if (ImGui::TreeNodeEx(transform->GetName().c_str()))
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