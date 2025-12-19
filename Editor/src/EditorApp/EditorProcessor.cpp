#include "EditorProcessor.h"
#include "Twisted/BuiltIn/BuiltInRegistry.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/GameService.h"
#include "Data/Color.h"
#include "Twisted/Gameing/World.h"
#include "EditorData/EditorConstants.h"
#include "Twisted/Windowing/WindowsService.h"
#include "UI/UIService.h"
#include "Twisted/Windowing/WindowCoreAPI.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include "Twisted/AssetsLayer/Project.h"
#include "EditorWorldService.h"

namespace Twisted::Editor
{
	void EditorProcessor::OnInit()
	{
		m_assetsService = m_app->AddService<AssetsService>();
		m_editorService = m_app->AddService<EditorWorldService>();
		m_uiService = m_app->AddService<UIService>();
		m_windowsService = m_app->AddService<WindowsService>();

		m_editorService->GetConfig().LoadConfig();

		m_editorService->ConfirmedQuitEvent.AddListener([this]() {
			m_app->Stop();
			});
	}
	void EditorProcessor::OnBeforeRun()
	{
		m_app->GetService<GameService>()->WorldChangeEvent.AddListener([this]([[maybe_unused]] World* world) {
			m_editorService->GetSelection().ClearEntities();
			});

		m_assetsService->GetProject().ProjectChangeEvent.AddListener([this]([[maybe_unused]] const Project& project) {
			m_app->GetService<WindowsService>()->GetWindow()->
				SetTitle(Constants::EDITOR_WINDOW_TITLE + " " + m_assetsService->GetProject().GetName());

			Im::SetLayoutIniFile(m_assetsService->GetProject().GetLayoutFilePath(), true);

			Collections::BuiltInRegistry::GetInstance().ImportAll();
			m_assetsService->AutoImportAssets();
			});

		CreateAppWindow();
		m_uiService->Init(m_windowsService->GetWindow());
	}

	void EditorProcessor::OnFrameBegin()
	{
		if (m_windowsService->GetWindow())
		{
			Color clearColor{}; //TODO... move this
			m_windowsService->GetWindow()->GetContext()->Clear(clearColor);
			Windowing::PollEvents(*m_windowsService->GetWindow());
		}

		m_editorService->GetInput().Update();
	}

	void EditorProcessor::OnFrame()
	{
		if (m_editorService->EditorWorld)
		{
			m_editorService->EditorWorld->UpdateFrame(1.0f); //TODO....
			m_editorService->renderer.Render(*m_editorService->EditorWorld, *m_app->GetService<GameService>()->GameWorld);
		}
		m_uiService->Render(m_windowsService->GetWindow());
	}
	void EditorProcessor::OnFrameEnd()
	{
		if (m_windowsService->GetWindow())
			m_windowsService->GetWindow()->GetContext()->SwapBuffers();
	}
	void EditorProcessor::OnTerminate()
	{
		m_editorService->SaveEditor(m_windowsService->GetWindow());

		if (m_windowsService->GetWindow())
			m_windowsService->DestroyWindow(m_windowsService->GetWindow());
	}

	void EditorProcessor::CreateAppWindow()
	{
		m_windowsService->CreateNewWindow(Constants::EDITOR_WINDOW_TITLE, m_editorService->GetConfig().GetWindowSize(), m_editorService->GetConfig().GetWindowPos());
		m_windowsService->GetWindow()->CloseWindowEvent.AddListener([this]() { m_app->Stop(); });
		m_windowsService->GetWindow()->SetTitle(Constants::EDITOR_WINDOW_TITLE);
		m_windowsService->GetWindow()->Maximize();
		m_windowsService->GetWindow()->SetPosition(m_editorService->GetConfig().GetWindowPos());
	}
}