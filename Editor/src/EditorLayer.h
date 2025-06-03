#pragma once
#include "AppCore.h"
#include "Twisted/Application/Layer.h"
#include "Project/Project.h"
#include "Twisted/Game/World.h"
#include "Configurator.h"
#include "UI/Panels/EditorPanel.h"
#include <string>

namespace Twisted::Editor
{
	class EditorLayer :public Twisted::Layer
	{
	public:
		EditorLayer(Application* app) :Twisted::Layer(app) {}

		void SetActiveWorld(SRef<World> world);
		World* GetActiveWorld() { return (m_world) ? m_world.get() : nullptr; }

		void SetActiveProject(SRef<Project> newProject);
		SRef<Project> GetActiveProject() { return m_activeProject; }

		EntityID GetSelectedEntity()const { return m_selectedEntityID; }
		void SetSelectedEntity(EntityID selectedEntity);

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

		void OnInit()override;
		void OnBeforeRun()override;
		void OnFrame()override;

	private:

		void LoadResources();
		void RenderDockSpace();
		void RenderMenuBar();

		std::vector<URef<EditorPanel>> Panels;
		Configurator m_configurator;
		SRef<Project> m_activeProject;
		SRef<World> m_world = nullptr;

		EntityID m_selectedEntityID = NullEntity;
	};
}