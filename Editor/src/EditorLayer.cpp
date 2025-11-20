#include "EditorLayer.h"

#include "Twisted/Gameing/Systems/RenderSystem.h"

#include "Twisted/Windowing/WindowLayer.h"
#include "Twisted/Rendering/RenderLayer.h"

#include "EditorRegistry.h"

#include "Twisted/Application/Application.h"
#include "UI/ImguiExtensions.h"

#include "Twisted/Gameing/Components/CCamera.h"
#include <Twisted/Gameing/Entity.h>
#include <Twisted/TObject.h>
#include <Twisted/Gameing/World.h>

namespace Twisted::Editor
{
	void EditorLayer::Init()
	{
		Im::Init(m_app->GetLayer<WindowLayer>()->GetWindow());

		for (auto& panel : EditorRegistry::GetInstance().m_panels)
			panel->Init();

		SetGameWorld(nullptr);
	}

	void EditorLayer::SaveEditor()
	{
		Window* window = m_app->GetLayer<WindowLayer>()->GetWindow();
		GetConfig().SetWindowPos(window->GetPosition());
		GetConfig().SetWindowSize(window->GetSize());
		GetConfig().SaveConfig();
	}

	void EditorLayer::InitEditorWorld()
	{
		m_editorWorld = TObject::Create<World>("Editor world"); //TODO...
		Entity camEnt = m_editorWorld->CreateNewEntityWithComponents<CameraComponent>();
	}

	void EditorLayer::SetGameWorld(World* world)
	{
		if (m_gameWorld)
		{
			TObject::Destroy(m_gameWorld);
			m_selection.ClearEntities();
		}

		if (!world)
		{
			world = TObject::Create<World>("New world"); //m_app
			world->AddSystem<RenderSystem>();
		}

		m_gameWorld = world;
		WorldChangeEvent.Invoke();
	}

	void EditorLayer::Update()
	{
		if (GetGameWorld())
			GetGameWorld()->UpdateFrame();
	}






}
