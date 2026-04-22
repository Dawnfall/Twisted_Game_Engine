#pragma once

#include "Twisted/Application/Service.h"
#include "Twisted/Windowing/Window.h"

#include "Twisted/Windowing/WindowsService.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include "Twisted/Gameing/GameService.h"
#include "Twisted/Application/TimeService.h"
#include "Twisted/Rendering/RenderService.h"

#include "Twisted/Rendering/Data/RenderContext.h"
#include "Twisted/Rendering/Data/ClearParams.h"

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
		EditorService(Application* app,int priority) :
			Service(app,priority)
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

		void OnInit()override;
		void OnBeforeRun()override;
		void OnFrameBegin()override;
		void OnFrame()override;
		void OnFrameEnd()override;
		void OnTerminate()override;

	private:
		void CreateAppWindow();
		void SaveEditorLayout();
		void LoadLastWorld(AssetUuid wolrdUuid);
		void SaveWorld(World* world);

		WindowsService* m_windowsService = nullptr;
		AssetsService* m_assetsService = nullptr;
		TimeService* m_timeService = nullptr;
		GameService* m_gameService = nullptr;
		RenderService* m_renderService = nullptr;

		ClearParams m_clearParams;
		unsigned int m_clearBits = 0;

	public:
	
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
