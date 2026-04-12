#include "EditorProcessor.h"
#include "Twisted/BuiltIn/BuiltInRegistry.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/GameService.h"
#include "Data/Color.h"
#include "Twisted/Gameing/World.h"
#include "EditorConstants.h"
#include "Twisted/Windowing/WindowsService.h"
#include "Twisted/Windowing/WindowCoreAPI.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include "Twisted/AssetsLayer/Project.h"
#include "Twisted/AssetsLayer/ProjectConfig.h"
#include "EditorWorld/EditorCameraSystem.h"
#include "Twisted/Rendering/RenderConvert.h"
#include "EditorApp/EditorRegistry.h"

#include <imgui.h>

namespace Twisted::Editor
{
	void EditorProcessor::OnInit()
	{
		m_assetsService = m_app->AddService<AssetsService>();
		m_editorService = m_app->AddService<EditorService>();
		m_windowsService = m_app->AddService<WindowsService>();
		m_gameService = m_app->AddService<GameService>();
		m_timeService = m_app->AddService<TimeService>();
		m_renderService = m_app->AddService<RenderService>();

		EditorConfig::GetInstance().LoadConfig();

		m_editorService->ConfirmedQuitEvent.AddListener([this]() {
			m_app->Stop();
			});

		m_gameService->WorldChangeEvent.AddListener([this](World* world) {
			m_editorService->GetSelection().ClearEntities();
			SaveWorld(world);
			m_editorService->WorldLoadedEvent.Invoke(world);
			});

		m_assetsService->ProjectChangeEvent.AddListener([this](const Project& prevProject, const Project& newProject) {
			m_app->GetService<WindowsService>()->GetWindow()->
				SetTitle(Constants::EDITOR_WINDOW_TITLE + " " + m_assetsService->GetProject().GetName());

			Collections::BuiltInRegistry::GetInstance().ImportAll();
			m_assetsService->AutoImportAssets();

			auto& config = m_assetsService->GetProject().GetConfig();
			m_editorService->InitPanels();
			if (prevProject.IsValid())
				config.Save();
			LoadLastWorld(newProject.GetConfig().lastWorld);
			});
	}

	void EditorProcessor::OnBeforeRun()
	{
		CreateAppWindow();
		m_editorService->CreateWorld();
		m_editorService->SetWindow(m_windowsService->GetWindow());
	}

	void EditorProcessor::OnFrameBegin()
	{
		m_windowsService->PollEvents();
		if (m_windowsService->GetWindow())
		{
			Color clearColor{}; //TODO... move this
			//m_windowsService->GetWindow()->Clear(clearColor);
		}
	}

	void EditorProcessor::OnFrame()
	{
		m_editorService->GetEditorWorld()->UpdateFrame(m_timeService->GetDeltaTime());

		if (m_gameService->GetGameWorld())
		{
			RenderContext gamecontext;
			gamecontext.camDatas   = CollectCameraData(*m_gameService->GetGameWorld());
			gamecontext.modelDatas = CollectModelData(*m_gameService->GetGameWorld());
			gamecontext.lightData  = CollectLightData(*m_gameService->GetGameWorld());
			m_renderService->ForwardRender(gamecontext);
		}
		if (m_editorService->GetEditorWorld() && m_gameService->GetGameWorld())
		{
			RenderContext editorContext;
			editorContext.camDatas = CollectCameraData(*m_editorService->GetEditorWorld());
			editorContext.modelDatas = CollectModelData(*m_gameService->GetGameWorld());
			editorContext.lightData = CollectLightData(*m_gameService->GetGameWorld());
			m_renderService->ForwardRender(editorContext);
		}
		m_editorService->Render(m_windowsService->GetWindow());
	}

	void EditorProcessor::OnFrameEnd()
	{
		if (m_windowsService->GetWindow())
			m_windowsService->GetWindow()->SwapBuffers();
	}

	void EditorProcessor::OnTerminate()
	{
		if (m_assetsService->GetProject().IsValid())
			m_assetsService->GetProject().GetConfig().Save();

		SaveEditorLayout();
		m_editorService->SaveEditor(m_windowsService->GetWindow());

		if (m_windowsService->GetWindow())
			m_windowsService->DestroyWindow(m_windowsService->GetWindow());
	}

	void EditorProcessor::SaveEditorLayout()
	{
		if (!m_assetsService->GetProject().IsValid())
			return;

		auto& config = m_assetsService->GetProject().GetConfig();

		size_t iniSize = 0;
		config.imguiIni = ImGui::SaveIniSettingsToMemory(&iniSize);
		config.panelStates.clear();
		for (const auto& panel : EditorRegistry::GetInstance().m_panels)
			config.panelStates.push_back({ panel->GetName(), panel->IsShowing });
		config.Save();
	}

	void EditorProcessor::CreateAppWindow()
	{
		auto& config = EditorConfig::GetInstance();
		m_windowsService->CreateNewWindow(Constants::EDITOR_WINDOW_TITLE, config.GetWindowSize(), config.GetWindowPos());
		Window* window = m_windowsService->GetWindow();
		window->CloseWindowEvent.AddListener([this]() { m_app->Stop(); });
		window->SetTitle(Constants::EDITOR_WINDOW_TITLE);
		if (config.GetWindowMaximized())
			window->Maximize();
	}


	void EditorProcessor::SaveWorld(World* world)
	{
		if (m_assetsService->GetProject().IsValid())
		{
			auto& config = m_assetsService->GetProject().GetConfig();
			config.lastWorld = m_assetsService->GetObjectUuid(world);
			config.Save();
		}
	}

	void EditorProcessor::LoadLastWorld(AssetUuid worldUuid)
	{
		if (!m_gameService->LoadWorld(worldUuid))
			m_gameService->NewGameWorld();
	}
}
