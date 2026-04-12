#pragma once

#include "Twisted/Application/Service.h"
#include "Twisted/Windowing/Window.h"

#include "EditorData/EditorConfig.h"
#include "EditorData/Selection.h"
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
			Service(app)
		{}
		~EditorService();

		Selection& GetSelection() { return m_selection; }
		UIWindow& GetUIWindow() { return m_uiWindow; }

		World* GetEditorWorld() { return m_editorWorld; }
		World* NewEditorWorld(const std::string& name = "Editor World");

		void SaveEditor(Window* window);

		void InitPanels();

		void Render(Window* window);
		void SetWindow(Window* window);

		void CreateWorld();

	public:
	
		Event<World*> WorldLoadedEvent; // Fired when the active game world changes (including on auto-load at project open)
		Event<fs::path> MakeNewFileEvent; //extension
		Event<> ConfirmedQuitEvent;

	private:
		UIWindow m_uiWindow;
		Selection m_selection;

		World* m_editorWorld = nullptr;
		Entity m_editorCameraEnt = Entity::Invalid();

		Window* m_window = nullptr;
	};
}
