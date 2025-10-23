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
		DragPayload& GetDragPayload() { return m_dragPayload; }
		
		void SetWorld(World* world)
		{
			//GetSelection().Clear();
			if (m_gameWorld)
			{
				TObject::Destroy(m_gameWorld);
			}

			if (!world)
				world = TObject::Create<World>(); //m_app

			m_gameWorld = world;
			//m_editorWorld = TObject::Create<World>(); //TODO...
			WorldChangeEvent.Invoke();
		}


	public:
		WPtr<FrameBuffer> GameViewBuffer;
		WPtr<FrameBuffer> EditorViewBuffer;

	private:
		Event<> WorldChangeEvent;

		World* m_gameWorld = nullptr;
		World* m_editorWorld = nullptr;
		Selection m_selection;
		EditorInput m_input;
		EditorConfig m_editorConfig;
		DragPayload m_dragPayload;

	private:
		EditorData() = default;
		EditorData(const EditorData& other) = delete;
		EditorData(EditorData&& other) = delete;
		EditorData& operator=(const EditorData& other) = delete;
		EditorData& operator=(EditorData&& other) = delete;
	};
}