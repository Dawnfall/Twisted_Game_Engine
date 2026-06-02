#include "EditorApp/EditorService.h"
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
#include "WindowCoreAPI.h"
#include "AssetsService.h"
#include "Project.h"
#include "ProjectConfig.h"
#include "RenderConvert.h"
#include "RenderAPI.h"

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

		m_gameService->BeforeWorldRestoreEvent.AddListener([this]() {
			m_renderService->WaitIdle();
			});

		m_assetsService->ProjectChangeEvent.AddListener([this](const Project& newProject) {

			m_renderService->GetWindow()->
				SetTitle(Constants::EDITOR_WINDOW_TITLE + " " + m_assetsService->GetProject().GetName());
			InitPanels();
			LoadLastWorld(newProject.GetConfig().lastWorld);
			});
	}

	void EditorService::OnBeforeRun()
	{
		CreateAppWindow();
		CreateWorld();
		SetWindow(m_renderService->GetWindow());
	}

	void EditorService::OnFrameBegin()
	{
		m_renderService->PollEvents();
		if (m_renderService->GetWindow())
		{
			//m_renderService->GetWindow()->Clear(Color{});
		}
	}

	void EditorService::OnFrame()
	{
		// Execute deferred world restore before any command buffer recording this frame.
		if (m_gameService->HasPendingRestore())
			m_gameService->ExecuteRestore();

		m_assetsService->ProcessWatchedChanges();
		GetEditorWorld()->UpdateFrame(m_timeService->GetDeltaTime());

		// Apply deferred framebuffer resizes before BeginFrame — safe to destroy GPU resources here.
		for (auto& panel : EditorRegistry::GetInstance().m_panels)
			panel->PreRender();

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
		if (GetEditorWorld())
		{
			RenderContext editorContext;
			editorContext.camDatas = CollectCameraData(*GetEditorWorld());
			if (m_gameService->GetGameWorld())
			{
				editorContext.modelDatas = CollectModelData(*m_gameService->GetGameWorld());
				editorContext.lightData = CollectLightData(*m_gameService->GetGameWorld());
				editorContext.lightData.ambient = ambient;
			}
			m_renderService->SubmitContext(editorContext);
		}
		m_renderService->Render();
		WorldViewRect = {};
		Render(m_renderService->GetWindow());
	}

	void EditorService::OnFrameEnd()
	{
	}

	void EditorService::OnTerminate()
	{
		if (m_assetsService->GetProject().IsValid())
			m_assetsService->GetProject().GetConfig().Save();

		SaveEditorLayout();
		SaveEditor(m_renderService->GetWindow());

		Im::Terminate();          // ImGui before Vulkan
		TObject::DestroyAll();    // GPU resources before vkDestroyDevice
		m_editorWorld = nullptr;  // DestroyAll freed it; prevent destructor double-destroy
		Render::Shutdown();       // Vulkan before window

		if (m_renderService->GetWindow())
			m_renderService->DestroyWindow(m_renderService->GetWindow());
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
		Window* window = m_renderService->CreateNewWindow(Constants::EDITOR_WINDOW_TITLE, config.GetWindowSize(), config.GetWindowPos());

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
			AssetUuid uuid = m_assetsService->GetObjectUuid(world);
			if (!uuid.IsValid())
				return;
			auto& config = m_assetsService->GetProject().GetConfig();
			config.lastWorld = uuid;
			config.Save();
		}
	}

	void EditorService::LoadLastWorld(AssetUuid worldUuid)
	{
		if (!m_gameService->LoadWorld(worldUuid))
			m_gameService->NewGameWorld();
	}

}
