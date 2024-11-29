#pragma once
#include "editorpch.h"
#include "Twisted/AppBase.h"
#include "UI/Panels/EditorPanel.h"
#include "Project/Project.h"
#include "UI/FileBrowser/FileBrowser.h"

namespace Twisted::Editor
{
	const std::string editorRootFolder = "F:/Programiranje/C++/GameEngine/TestGame/Assets/"; //TODO:...
	const std::string windowTitle = "Twisted Editor";
	const Twisted::Vec2i windowSize(1280, 720);

	class EditorApp :public AppBase
	{
	public:
		void Run(Twisted::RuntimeBase* runtime) override;

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
			if (m_activeProject)
				m_window->SetTitle("Twisted Editor: " + newProject->GetName());
		}

		std::shared_ptr<Project> GetActiveProject() { return m_activeProject; }

	protected:
		void InitIMGUI();
		void LoadResources();
		bool Init();
		void Terminate();

		void RenderUI();
		void RenderDockSpace();
		void RenderMenuBar();
		void RenderModals();
	protected:
		std::vector<std::unique_ptr<EditorPanel>> m_panels;
		EntityID m_selectedEntityID = NullEntity;
		std::shared_ptr<Project> m_activeProject;

		std::unique_ptr<FileBrowser> m_fileBrowser = nullptr;
	};

}