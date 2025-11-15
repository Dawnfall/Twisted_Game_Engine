#pragma once

#include "Twisted/Application/Layer.h"
#include "Utils/Event.h"

#include "LoadupConfig.h"
#include "EditorConfig.h"
#include "EditorData/Selection.h"
#include "EditorData/EditorInput.h"

#include "UI/UIWindow.h"
#include <Twisted/Gameing/World.h>

namespace Twisted::Editor
{
	class EditorLayer :public Layer
	{
	public:

		EditorLayer(Application* app): 
			Layer(app),
			m_loadupConfigData(Constants::LOADUP_CONFIG_PATH.string())
		{
			s_instance = this;
		}

		static EditorLayer& GetInstance()
		{		
			return *s_instance;
		}

		void Init();			
		void Update();
		void SaveEditor();
		void SetWorld(World* world);

		World* GetGameWorld() { return m_gameWorld; }
		World* GetEditorWorld() { return m_editorWorld; }
		Selection& GetSelection() { return m_selection; }
		EditorInput& GetInput() { return m_input; }
		EditorConfig& GetConfig() { return m_editorConfig; }

		LoadupConfig& GetLoadupConfig() { return m_loadupConfigData; }
		UIWindow& GetUIWindow() { return m_uiWindow; }

	public:

		Event<fs::path> MakeNewFileEvent; //extension
		Event<fs::path> SelectWorldPath;
		Event<fs::path> SelectedProjectPath;
		Event<fs::path> SaveWorldPath;
		Event<fs::path> CreateNewAssetPath;

		Event<> ConfirmedQuitEvent;
		Event<> WorldChangeEvent;

	private:

		inline static EditorLayer* s_instance = nullptr;

		World* m_gameWorld = nullptr;
		World* m_editorWorld = nullptr;

		Selection m_selection;
		EditorInput m_input;
		UIWindow m_uiWindow;

		LoadupConfig m_loadupConfigData;
		EditorConfig m_editorConfig;
	};
}
