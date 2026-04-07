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
#include "EditorWorld/EditorCameraSystem.h"
#include "UI/LayoutManager.h"
#include "Twisted/Rendering/RenderConvert.h"

namespace Twisted::Editor
{
	void EditorProcessor::OnInit()
	{
		m_assetsService = m_app->AddService<AssetsService>();
		m_editorEventsService = m_app->AddService<EditorService>();
		m_windowsService = m_app->AddService<WindowsService>();
		m_gameService = m_app->AddService<GameService>();
		m_timeService = m_app->AddService<TimeService>();
		m_renderService = m_app->AddService<RenderService>();
		 
		m_editorEventsService->GetConfig().LoadConfig();

		m_editorEventsService->ConfirmedQuitEvent.AddListener([this]() {
			m_app->Stop();
			});
	}
	void EditorProcessor::OnBeforeRun()
	{
		m_app->GetService<GameService>()->WorldChangeEvent.AddListener([this](World* world) {
			m_gameWorld = world;
			m_editorEventsService->GetSelection().ClearEntities();

			ProjectConfig* config = m_editorEventsService->GetProjectConfig();
			if (config)
			{
				AssetUuid uuid = m_gameService->GetGameWorldUuid();
				if (uuid.IsValid())
					config->SetLastWorld(uuid);
				else
					config->ClearLastWorld();
				config->Save();
			}

			m_editorEventsService->WorldLoadedEvent.Invoke(world);
			});

		m_assetsService->ProjectChangeEvent.AddListener([this](const Project& project) {
			m_app->GetService<WindowsService>()->GetWindow()->
				SetTitle(Constants::EDITOR_WINDOW_TITLE + " " + m_assetsService->GetProject().GetName());

			LayoutManager::GetInstance().LoadLayout(m_assetsService->GetProject().GetPanelLayoutPath());

			m_editorEventsService->LoadProjectConfig(project.GetProjectFilePath());

			Collections::BuiltInRegistry::GetInstance().ImportAll();
			m_assetsService->AutoImportAssets();

			// Auto-load last active world
			ProjectConfig* config = m_editorEventsService->GetProjectConfig();
			if (config)
			{
				AssetUuid lastWorldUuid = config->GetLastWorldUuid();

						m_gameService->LoadWorld(lastWorldUuid, *m_assetsService);
			}

			m_editorEventsService->ProjectLoadedEvent.Invoke(project);
			});

		CreateAppWindow();

		m_gameWorld = m_gameService->NewGameWorld();

		m_editorWorld = m_editorEventsService->NewEditorWorld("Editor World");
		m_editorCameraEnt = m_editorWorld->CreateNewEntityWithComponents<CameraComponent>();
		m_editorWorld->AddSystem<EditorCameraSystem>();

		m_editorEventsService->Init(m_windowsService->GetWindow());
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
		RenderContext gameContext = ExtractContext(*m_gameWorld);
		m_renderService->ForwardRender(gameContext);

		m_editorEventsService->Render(m_windowsService->GetWindow());
	}
	void EditorProcessor::OnFrameEnd()
	{
		if (m_windowsService->GetWindow())
			m_windowsService->GetWindow()->SwapBuffers();
	}
	void EditorProcessor::OnTerminate()
	{
		if (m_assetsService->GetProject().IsValid())
			LayoutManager::GetInstance().SaveLayout(m_assetsService->GetProject().GetPanelLayoutPath());

		m_editorEventsService->SaveEditor(m_windowsService->GetWindow());

		if (m_windowsService->GetWindow())
			m_windowsService->DestroyWindow(m_windowsService->GetWindow());

	}

	void EditorProcessor::CreateAppWindow()
	{
		auto& config = m_editorEventsService->GetConfig();
		m_windowsService->CreateNewWindow(Constants::EDITOR_WINDOW_TITLE, config.GetWindowSize(), config.GetWindowPos());
		Window* window = m_windowsService->GetWindow();
		window->CloseWindowEvent.AddListener([this]() { m_app->Stop(); });
		window->SetTitle(Constants::EDITOR_WINDOW_TITLE);
		if (config.GetWindowMaximized())
			window->Maximize();
	}

}