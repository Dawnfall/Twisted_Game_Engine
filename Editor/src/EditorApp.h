#pragma once
#include "editorpch.h"
#include "Twisted/AppBase.h"
#include "UI/Panels/EditorPanel.h"
#include "Project/Project.h"

namespace Twisted
{
	class Entity;
}

namespace Twisted::Editor
{

	class EditorApp :public AppBase
	{
	public:
		void SelectEntity(EntityID selectedEntity)
		{
			if (m_selectedEntityID == selectedEntity)
				return;
			m_selectedEntityID = selectedEntity;
		}
		EntityID GetSelectedEntity()const { return m_selectedEntityID; }

		template<typename T>
		void CreateEditorPanel()
		{
			static_assert(std::is_base_of<EditorPanel, T>::value, "T must be derived from EditorPanel");
			auto newPanel = std::make_unique<T>();
			m_panels.emplace_back(std::move(newPanel));
		}

		void SetActiveProject(std::shared_ptr<Project> newProject)
		{
			if (newProject == m_activeProject)
				return;

			m_activeProject = newProject;
		}
		std::shared_ptr<Project> GetActiveProject() { return m_activeProject; }
	protected:
		void CreateNewWindow(const AppParams& params) override;
		void RenderUI();
		void Run() override;

		void RenderDockSpace();
		void RenderMenuBar();
		void RenderCreateProjectWindow();
		void RenderOpenProjectWindow();

		virtual void LoadResources(const AppParams& params)override;

	protected:
		std::vector<std::unique_ptr<EditorPanel>> m_panels;
		EntityID m_selectedEntityID = NullEntity;
		std::shared_ptr<Project> m_activeProject;
	};

}