#pragma once
#include "Twisted/Application/RuntimeBase.h"
#include "Game/GameLayer.h"
#include "Rendering/RenderLayer.h"
#include "Windowing/WindowLayer.h"

#include "Project/Project.h"
#include "Configurator.h"
#include "UI/EditorPanel.h"
#include "Project/ProjectLoader.h"

namespace Twisted::Editor
{
	class EditorRuntime :public RuntimeBase
	{
	public:
		EntityID GetSelectedEntity()const { return m_selectedEntityID; }
		void SetSelectedEntity(EntityID selectedEntity);

		void SetActiveProject(SRef<Project> project) { m_activeProject = project; }
		Project* GetActiveProject() { return (m_activeProject) ? m_activeProject.get() : nullptr; }

		void RenderDockSpace();
		void RenderMenuBar();

		Window* GetWindow() { return m_window; }
		World* GetGameWorld() { return m_gameWorld; }
		World* GetEditorWorld() { return m_editorWorld; }

		// Event<> ProjectChangeEvent;
		Event<> WorldChangeEvent;

		template<typename T>
		void CreateEditorPanel()
		{
			static_assert(std::is_base_of<EditorPanel, T>::value, "T must be derived from EditorPanel");
			Panels.emplace_back(std::make_unique<T>(this));
		}

		template<typename T>
		T* GetPanel()
		{
			static_assert(std::is_base_of<EditorPanel, T>::value, "T must be derived from EditorPanel");
			for (auto& panel : Panels)
				if (T* castedPanel = dynamic_cast<T*>(panel.get()))
					return castedPanel;
			return nullptr;
		}

		void CreateNewWorld();
		void LoadWorld(const std::filesystem::path& path);
		void SaveWorldAs(const std::filesystem::path& path);
	private:
		URef<ProjectLoader> m_projectLoader = nullptr;
		SRef<Project> m_activeProject = nullptr;
		std::vector<URef<EditorPanel>> Panels;

		Configurator m_configurator;
		EntityID m_selectedEntityID = NullEntity;

		WindowLayer* m_windowLayer = nullptr;
		Render::RenderLayer* m_renderLayer = nullptr;
		GameLayer* m_gameLayer = nullptr;

		Window* m_window = nullptr;

		World* m_gameWorld = nullptr;
		World* m_editorWorld = nullptr;

	public:
		void OnCreate()override;
		void OnInit()override;
		void OnBeforeRun()override;
		void OnFrame()override;
	};


}