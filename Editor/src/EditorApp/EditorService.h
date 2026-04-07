#pragma once

#include "Twisted/Application/Service.h"
#include "Twisted/Windowing/Window.h"

#include "EditorData/EditorConfig.h"
#include "EditorData/Selection.h"
#include "EditorData/LoadupConfig.h"
#include "EditorData/ProjectConfig.h"
#include "Utils/Event.h"
#include "UI/UIWindow.h"

namespace Twisted
{
	class Project;
	class World;
}

namespace Twisted::Editor
{
	class EditorService : public Service
	{
	public:
		EditorService(Application* app) :
			Service(app),
			m_loadupConfigData(Constants::LOADUP_CONFIG_PATH.string())
		{}
		~EditorService();

		EditorConfig& GetConfig() { return m_editorConfig; }
		LoadupConfig& GetLoadupConfig() { return m_loadupConfigData; }
		ProjectConfig* GetProjectConfig() { return m_projectConfig ? &*m_projectConfig : nullptr; }

		Selection& GetSelection() { return m_selection; }
		UIWindow& GetUIWindow() { return m_uiWindow; }

		World* GetEditorWorld() { return m_editorWorld; }
		World* NewEditorWorld(const std::string& name = "Editor World");

		void SaveEditor(Window* window);
		void LoadProjectConfig(const fs::path& projectFilePath) { m_projectConfig.emplace(projectFilePath); }

		// Fired after a project is fully loaded (built-ins imported, assets imported, last world restored)
		Event<const Project&> ProjectLoadedEvent;

		// Fired when the active game world changes (including on auto-load at project open)
		Event<World*> WorldLoadedEvent;

		void Init(Window* window);
		void Render(Window* window);

	public:
		Event<fs::path> MakeNewFileEvent; //extension
		Event<> ConfirmedQuitEvent;

	private:
		
		UIWindow m_uiWindow;

		Selection m_selection;
		LoadupConfig m_loadupConfigData;
		EditorConfig m_editorConfig;
		std::optional<ProjectConfig> m_projectConfig;

		World* m_editorWorld = nullptr;
		Window* m_window = nullptr;
	};
}
