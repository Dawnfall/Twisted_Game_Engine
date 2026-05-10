#include "EditorService.h"
#include "Application/TObject.h"
#include "World.h"
#include "Window.h"
#include "UI/ImguiExtensions.h"
#include "EditorApp/EditorRegistry.h"
#include "EditorWorld/EditorCameraSystem.h"

#include "BuiltIn/BuiltInRegistry.h"
#include "GameService.h"
#include "Data/Color.h"
#include "EditorConstants.h"
#include "WindowsService.h"
#include "WindowCoreAPI.h"
#include "AssetsService.h"
#include "Project.h"
#include "ProjectConfig.h"
#include "RenderConvert.h"

#include <imgui.h>

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

	void EditorService::CreateWorld()
	{
		m_editorWorld = NewEditorWorld("Editor World");
		m_editorCameraEnt = m_editorWorld->CreateNewEntityWithComponents<CameraComponent>();
		m_editorWorld->AddSystem<EditorCameraSystem>();
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

	void EditorService::SetWindow(Window* window)
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


	void EditorService::OnInit()
	{
		m_timeService = m_app->GetService<TimeService>();
		m_gameService = m_app->GetService<GameService>();
		m_windowsService = m_app->GetService<WindowsService>();
		m_renderService = m_app->GetService<RenderService>();
		m_assetsService = m_app->GetService<AssetsService>();

		EditorConfig::GetInstance().LoadConfig();

		ConfirmedQuitEvent.AddListener([this]() {
			m_app->Stop();
			});

		m_gameService->WorldChangeEvent.AddListener([this](World* world) {
			GetSelection().ClearEntities();
			SaveWorld(world);
			});

		m_assetsService->ProjectChangeEvent.AddListener([this](const Project& newProject) {
			
			m_app->GetService<WindowsService>()->GetWindow()->
				SetTitle(Constants::EDITOR_WINDOW_TITLE + " " + m_assetsService->GetProject().GetName());
			InitPanels();
			LoadLastWorld(newProject.GetConfig().lastWorld);
			});
	}

	void EditorService::OnBeforeRun()
	{
		CreateAppWindow();
		CreateWorld();
		SetWindow(m_windowsService->GetWindow());
	}

	void EditorService::OnFrameBegin()
	{
		m_windowsService->PollEvents();
		if (m_windowsService->GetWindow())
		{
			//m_windowsService->GetWindow()->Clear(Color{});
		}
	}

	void EditorService::OnFrame()
	{
		m_assetsService->ProcessWatchedChanges();
		GetEditorWorld()->UpdateFrame(m_timeService->GetDeltaTime());

		const Vec4f ambient = m_assetsService->GetProject().GetConfig().ambientLight;
		if (m_gameService->GetGameWorld())
		{
			RenderContext gamecontext;
			gamecontext.camDatas = CollectCameraData(*m_gameService->GetGameWorld());
			gamecontext.modelDatas = CollectModelData(*m_gameService->GetGameWorld());
			gamecontext.lightData = CollectLightData(*m_gameService->GetGameWorld());
			gamecontext.lightData.ambient = ambient;
			m_renderService->SubmitContext(gamecontext);
		}
		if (GetEditorWorld() && m_gameService->GetGameWorld())
		{
			RenderContext editorContext;
			editorContext.camDatas = CollectCameraData(*GetEditorWorld());
			editorContext.modelDatas = CollectModelData(*m_gameService->GetGameWorld());
			editorContext.lightData = CollectLightData(*m_gameService->GetGameWorld());
			editorContext.lightData.ambient = ambient;
			m_renderService->SubmitContext(editorContext);
		}
		m_renderService->Render();
		WorldViewRect = {};
		Render(m_windowsService->GetWindow());
	}

	void EditorService::OnFrameEnd()
	{
		m_windowsService->GetWindow()->SwapBuffers();
	}

	void EditorService::OnTerminate()
	{
		if (m_assetsService->GetProject().IsValid())
			m_assetsService->GetProject().GetConfig().Save();

		SaveEditorLayout();
		SaveEditor(m_windowsService->GetWindow());

		if (m_windowsService->GetWindow())
			m_windowsService->DestroyWindow(m_windowsService->GetWindow());
	}

	void EditorService::SaveEditorLayout()
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

	void EditorService::CreateAppWindow()
	{
		auto& config = EditorConfig::GetInstance();
		Window* window = m_windowsService->CreateNewWindow(Constants::EDITOR_WINDOW_TITLE, config.GetWindowSize(), config.GetWindowPos());

		window->CloseWindowEvent.AddListener([this]() { m_app->Stop(); });
		window->FilesDroppedEvent.AddListener([this](const std::vector<fs::path>& paths) {
			if (!m_assetsService->GetProject().IsValid())
				return;
			const fs::path assetsRoot = m_assetsService->GetProject().GetAssetsFolder();
			for (const fs::path& src : paths)
			{
				fs::path dest = src;
				auto rel = fs::relative(src, assetsRoot);
				bool insideAssets = !rel.empty() && !rel.native().starts_with(L"..");
				if (!insideAssets)
				{
					dest = assetsRoot / src.filename();
					if (!fs::exists(dest))
						fs::copy_file(src, dest);
				}
				if (fs::exists(dest))
				{
					m_assetsService->RegisterAsset(dest);
					m_assetsService->Load(m_assetsService->GetUuid(dest));
				}
			}
			});
		window->SetTitle(Constants::EDITOR_WINDOW_TITLE);
		if (config.GetWindowMaximized())
			window->Maximize();
	}


	void EditorService::SaveWorld(World* world)
	{
		if (m_assetsService->GetProject().IsValid())
		{
			auto& config = m_assetsService->GetProject().GetConfig();
			config.lastWorld = m_assetsService->GetObjectUuid(world);
			config.Save();
		}
	}

	void EditorService::LoadLastWorld(AssetUuid worldUuid)
	{
		if (!m_gameService->LoadWorld(worldUuid))
			m_gameService->NewGameWorld();
	}

}
