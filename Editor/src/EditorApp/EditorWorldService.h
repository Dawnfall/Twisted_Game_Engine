#pragma once

#include "Twisted/Application/Service.h"
#include "Utils/Event.h"

#include "EditorData/LoadupConfig.h"
#include "EditorData/EditorConfig.h"
#include "EditorData/Selection.h"
#include "EditorData/EditorInput.h"

#include "Twisted/Rendering/Data/RenderContext.h"
#include "EditorWorld/EditorRenderer.h"
#include "Twisted/Gameing/GameService.h"
#include "Twisted/Windowing/WindowsService.h"

#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/Entity.h"
#include "EditorWorld/EditorCameraSystem.h"

namespace Twisted
{
	class World;
}

namespace Twisted::Editor
{

	class EditorWorldService :public Service
	{
	public:

		EditorWorldService(Application* app);

		inline static EditorWorldService* GetInstance()
		{
			return s_instance;
		}

		void SaveEditor(Window* window);

		Selection& GetSelection() { return m_selection; }
		EditorInput& GetInput() { return m_input; }
		EditorConfig& GetConfig() { return m_editorConfig; }

		LoadupConfig& GetLoadupConfig() { return m_loadupConfigData; }

	public:
		Event<fs::path> MakeNewFileEvent; //extension
		Event<> ConfirmedQuitEvent;

		World* EditorWorld = nullptr;
		Entity camEnt = Entity::Invalid();
		EditorRenderer renderer;

	private:

		inline static EditorWorldService* s_instance = nullptr;

		Selection m_selection;
		EditorInput m_input;

		LoadupConfig m_loadupConfigData;
		EditorConfig m_editorConfig;

		Window* m_window=nullptr;
	};
}
