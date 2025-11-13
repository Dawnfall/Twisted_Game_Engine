#include "EditorApplication.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/World.h"

inline static std::string layoutFilePath = "";

namespace Twisted::Editor
{
	void EditorApplication::OnInit()
	{
		m_windowLayer = AddLayer<WindowLayer>();
		m_renderLayer = AddLayer<RenderLayer>();
		m_assetsLayer = AddLayer<AssetsLayer>();
		m_editorLayer = AddLayer<EditorLayer>();
	}

	void EditorApplication::OnBeforeRun()
	{
		SetCallbacks();
		CreateAppWindow();

		m_editorLayer->Init();
	}

	void EditorApplication::OnBeginFrame()
	{
		m_windowLayer->PollEvents();

		m_editorLayer->GetInput().Update(); //TODO... this should not be here probably

		m_window->GetContext()->Clear(Constants::WINDOW_CLEAR_COLOR);

	}
	void EditorApplication::OnFrame()
	{
		m_editorLayer->GetUIWindow().Render(m_window);
	}
	void EditorApplication::OnEndFrame()
	{
		m_editorLayer->GetGameWorld()->UpdateFrame();
		m_renderLayer->Render();
		m_window->GetContext()->SwapBuffers();
	}
	void EditorApplication::OnTerminate()
	{
		m_editorLayer->SaveEditor();
		TObject::DestroyAll();
	}

	void EditorApplication::SetCallbacks()
	{
		m_editorLayer->ConfirmedQuitEvent.AddListener([this]() {
			Stop();
			});

		m_editorLayer->SelectedProjectPath.AddListener([this](fs::path path) {
			if (fs::is_regular_file(path))
				path = path.parent_path();

			if (m_assetsLayer->GetProject().SetProject(path))
				m_editorLayer->GetLoadupConfig().AddLatest(path.string());
			else
				m_editorLayer->GetLoadupConfig().RemoveEntry(path.string());
			m_editorLayer->GetLoadupConfig().Save();
			});

		m_editorLayer->SelectWorldPath.AddListener([this](fs::path path) {
			auto objects = m_assetsLayer->ImportAssetDirect(path);
			if (objects.size() == 1)
			{
				World* world = dynamic_cast<World*>(objects[0].GetObj());
				if (world)
					m_editorLayer->SetWorld(world);
			}
			});

		m_editorLayer->SaveWorldPath.AddListener([this](fs::path path) {
			std::vector<WPtrBase> assetObjects = { WPtrBase(m_editorLayer->GetGameWorld()) };
			m_assetsLayer->SaveAssetDirect(path, { assetObjects });
			});

		//layout saving
		m_assetsLayer->GetProject().ProjectChangeEvent.AddListener([this]() {
			layoutFilePath = (this->m_assetsLayer->GetProject().GetRootPath() / "EditorLayout.ini").string();
			Im::SetLayoutIniFile(layoutFilePath, true);
			});

		// window title
		m_assetsLayer->GetProject().ProjectChangeEvent.AddListener([this]() {
			m_windowLayer->GetWindow()->SetTitle(Constants::EDITOR_WINDOW_TITLE + " " + this->m_assetsLayer->GetProject().GetName());
			});

		//resource load
		m_assetsLayer->GetProject().ProjectChangeEvent.AddListener([this]() {
			m_assetsLayer->AutoImportAssets();
			m_assetsLayer->LoadBuiltIn();
			});

		m_editorLayer->CreateNewAssetPath.AddListener([this](fs::path path) {
			m_assetsLayer->CreateNewAsset(path);
			});
	}

	void EditorApplication::CreateAppWindow()
	{
		m_window = m_windowLayer->CreateNewWindow(Constants::LOADUP_WIN_TITLE, Constants::LOADUP_WIN_SIZE, Constants::LOADUP_WIN_POS);
		m_window->CloseWindowEvent.AddListener([this]() { Stop(); });
		m_window->SetTitle(Constants::EDITOR_WINDOW_TITLE);
		m_window->Maximize();
		m_window->SetPosition(EditorLayer::GetInstance().GetConfig().GetWindowPos());
	}
}
