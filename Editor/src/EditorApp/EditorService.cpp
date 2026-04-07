#include "EditorService.h"
#include "Twisted/TObject.h"
#include "Twisted/Gameing/World.h"
#include "Twisted/Windowing/Window.h"
#include "UI/ImguiExtensions.h"
#include "EditorApp/EditorRegistry.h"

namespace Twisted::Editor
{
	EditorService::~EditorService()
	{
		if (m_editorWorld)
			TObject::Destroy(m_editorWorld);
	}

	World* EditorService::NewEditorWorld(const std::string& name)
	{
		if (m_editorWorld)
			TObject::Destroy(m_editorWorld);
		m_editorWorld = TObject::Create<World>(name);
		return m_editorWorld;
	}


	void EditorService::SaveEditor(Window* window)
	{
		bool maximized = window->IsWindowMaximized();
		GetConfig().SetWindowMaximized(maximized);
		if (maximized)
		{
			auto restore = window->GetRestoreBounds();
			GetConfig().SetWindowPos(restore.pos);
			GetConfig().SetWindowSize(restore.size);
		}
		else
		{
			GetConfig().SetWindowPos(window->GetPosition());
			GetConfig().SetWindowSize(window->GetSize());
		}
		GetConfig().SaveConfig();
	}

	void EditorService::Init(Window* window)
	{
		Im::Init(window);
		for (auto& panel : EditorRegistry::GetInstance().m_panels)
			panel->Init();
	}

	void EditorService::Render(Window* window)
	{
		m_uiWindow.Render(window);
	}

}
