#pragma once

#include "Twisted/TObject.h"
#include "Utils/WPtr.h"
#include "Selection.h"
#include "Twisted/Gameing/World.h"
#include "Twisted/Rendering/FrameBuffer.h"
#include "Utils/Event.h"
#include "EditorInput.h"
#include "EditorConfig.h"
#include "DragPayload.h"
#include "Twisted/Gameing/Systems/RenderSystem.h"

namespace Twisted::Editor
{
	class EditorData
	{
	public:
		static EditorData& GetInstance()
		{
			static EditorData instance;
			return instance;
		}

	public:
		World* GetGameWorld() { return m_gameWorld; }
		World* GetEditorWorld() { return m_editorWorld; }
		Selection& GetSelection() { return m_selection; }
		EditorInput& GetInput() { return m_input; }
		EditorConfig& GetConfig() { return m_editorConfig; }
		
		void SetWorld(World* world)
		{
			if (m_gameWorld)
			{
				TObject::Destroy(m_gameWorld);
				EditorData::GetInstance().GetSelection().ClearEntities();
			}

			if (!world)
			{
				world = TObject::Create<World>("New world"); //m_app
				world->AddSystem<RenderSystem>();
			}

			m_gameWorld = world;
			//m_editorWorld = TObject::Create<World>(); //TODO...
			WorldChangeEvent.Invoke();
		}


	public:
		WPtr<FrameBuffer> GameViewBuffer;
		WPtr<FrameBuffer> EditorViewBuffer;

		Event<fs::path> MakeNewFileEvent; //extension
		Event<> WorldChangeEvent;

	private:


		World* m_gameWorld = nullptr;
		World* m_editorWorld = nullptr;
		Selection m_selection;
		EditorInput m_input;
		EditorConfig m_editorConfig;

	private:
		EditorData() = default;
		EditorData(const EditorData& other) = delete;
		EditorData(EditorData&& other) = delete;
		EditorData& operator=(const EditorData& other) = delete;
		EditorData& operator=(EditorData&& other) = delete;
	};
}