#include "EditorApplication.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/World.h"
#include <Twisted/TObject.h>

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
		m_editorLayer->GetConfig().LoadConfig();

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

	}

	void EditorApplication::CreateAppWindow()
	{
		m_window = m_windowLayer->CreateNewWindow(Constants::EDITOR_WINDOW_TITLE, m_editorLayer->GetConfig().GetWindowSize(), m_editorLayer->GetConfig().GetWindowPos());
		m_window->CloseWindowEvent.AddListener([this]() { Stop(); });
		m_window->SetTitle(Constants::EDITOR_WINDOW_TITLE);
		m_window->Maximize();
		m_window->SetPosition(m_editorLayer->GetConfig().GetWindowPos());

		m_renderLayer->Init();
	}
}
