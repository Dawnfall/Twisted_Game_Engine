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
		EditorConfig::GetInstance().SetWindowMaximized(maximized);
		if (maximized)
		{
			auto restore = window->GetRestoreBounds();
			EditorConfig::GetInstance().SetWindowPos(restore.pos);
			EditorConfig::GetInstance().SetWindowSize(restore.size);
		}
		else
		{
			EditorConfig::GetInstance().SetWindowPos(window->GetPosition());
			EditorConfig::GetInstance().SetWindowSize(window->GetSize());
		}
		EditorConfig::GetInstance().SaveConfig();
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

	void EditorService::InitPanels()
	{
		auto& config = m_app->GetService<AssetsService>()->GetProject().GetConfig();

		// Apply layout
		if (!config.imguiIni.empty())
			ImGui::LoadIniSettingsFromMemory(config.imguiIni.c_str(), config.imguiIni.size());
		for (const auto& ps : config.panelStates)
		{
			for (auto& panel : EditorRegistry::GetInstance().m_panels)
			{
				if (panel->GetName() == ps.name)
				{
					panel->IsShowing = ps.showing;
					break;
				}
			}
		}
	}

}
