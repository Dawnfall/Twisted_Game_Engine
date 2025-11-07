#pragma once

#include "Twisted/Application/Layer.h"
#include "Twisted/Rendering/FrameBuffer.h"
#include "LoadupConfig.h"
#include "EditorConstants.h"

#include "Utils/WPtr.h"
#include "Twisted/TObject.h"
#include "Twisted/Gameing/Systems/RenderSystem.h"
#include "Twisted/Gameing/World.h"
#include "Utils/Event.h"
#include "EditorConfig.h"
#include "EditorData/Selection.h"
#include "EditorData/EditorInput.h"
#include "EditorData/DragPayload.h"

namespace Twisted
{
	class Window;
	class Application;
}

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

		void Init() override;
		void Render(Window* window);

		World* GetGameWorld() { return m_gameWorld; }
		World* GetEditorWorld() { return m_editorWorld; }
		Selection& GetSelection() { return m_selection; }
		EditorInput& GetInput() { return m_input; }
		EditorConfig& GetConfig() { return m_editorConfig; }
		LoadupConfig& GetLoadupConfig() { return m_loadupConfigData; }

		void SetWorld(World* world)
		{
			if (m_gameWorld)
			{
				ObjectManager::Destroy(m_gameWorld);
				m_selection.ClearEntities();
			}

			if (!world)
			{
				world = ObjectManager::Create<World>("New world"); //m_app
				world->AddSystem<RenderSystem>();
			}

			m_gameWorld = world;
			//m_editorWorld = TObject::Create<World>(); //TODO...
			WorldChangeEvent.Invoke();
		}

	public:
		Event<fs::path> MakeNewFileEvent; //extension
		Event<> WorldChangeEvent;
	private:
		inline static EditorLayer* s_instance = nullptr;

		World* m_gameWorld = nullptr;
		World* m_editorWorld = nullptr;
		Selection m_selection;
		EditorInput m_input;
		EditorConfig m_editorConfig;
		LoadupConfig m_loadupConfigData;

		void RenderDockSpace();
		void RenderMenuBar(Window* window);
	};
}