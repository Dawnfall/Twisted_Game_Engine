#include "EditorLayer.h"

#include "Twisted/Gameing/Systems/RenderSystem.h"

#include "Twisted/Windowing/WindowLayer.h"
#include "Twisted/Rendering/RenderLayer.h"

#include "EditorRegistry.h"

#include "Twisted/Application/Application.h"
#include "UI/ImguiExtensions.h"

namespace Twisted::Editor
{
	void EditorLayer::Init()
	{
		Im::Init(m_app->GetLayer<WindowLayer>()->GetWindow());
		m_editorConfig.LoadConfig();
		//Im::SetLayoutIniFile(""); //TODO:... not sure we can do this some place else

		for (auto& panel : EditorRegistry::GetInstance().m_panels)
			panel->Init();

		SetWorld(nullptr);
	}

	void EditorLayer::SaveEditor()
	{
		Window* window = m_app->GetLayer<WindowLayer>()->GetWindow();
		GetConfig().SetWindowPos(window->GetPosition());
		GetConfig().SetWindowSize(window->GetSize());
		GetConfig().SaveConfig();
	}

	void EditorLayer::SetWorld(World* world)
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
		//m_editorWorld = TObject::Create<World>(); //TODO...
		WorldChangeEvent.Invoke();
	}

	void EditorLayer::Update()
	{
		if (GetGameWorld())
			GetGameWorld()->UpdateFrame();
	}






}
