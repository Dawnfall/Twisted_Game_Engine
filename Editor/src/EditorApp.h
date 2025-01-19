#pragma once
#include "Twisted/AppBase.h"
#include "Project/Project.h"
#include "Twisted/Game/World.h"
#include "UI/UIManager.h"
#include "Configurator.h"

namespace Twisted::Editor
{
	const std::string testProjectPath = "F:/Programiranje/Test/TestProjectA";
	class EditorApp :public AppBase
	{
	public:
		EditorApp();
		void Run() override;

		void SetActiveWorld(SRef<World> world);
		SRef<World> GetActiveWorld() { return m_world; }

		void SetActiveProject(SRef<Project> newProject);
		SRef<Project> GetActiveProject() { return m_activeProject; }

		EntityID GetSelectedEntity()const { return m_selectedEntityID; }
		void SetSelectedEntity(EntityID selectedEntity);

	protected:

		bool Init();
		void LoadResources();
		void Terminate();

	protected:

		UIManager m_uiManager;
		Configurator m_configurator;
		SRef<Project> m_activeProject;
		SRef<World> m_world = nullptr;

		EntityID m_selectedEntityID = NullEntity;
	};

}
