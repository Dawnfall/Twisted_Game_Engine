#include "editorpch.h"
#include "EditorApp.h"
#include "Rendering/RenderingAPI.h"
#include "UI/ImguiExtensions.h"
#include "Windows/Window.h"
#include "AppCore.h"
#include <GLFW/glfw3.h>
#include "EditorConfig.h"


namespace Twisted::Editor
{
	EditorApp::EditorApp() :
		m_uiManager(this)
	{
	}

	void EditorApp::Run()
	{
		Init();

		m_configurator.LoadConfig();

		m_monitor = Monitor::CreateMonitorInfo();
		m_window = Window::CreateNewWindow(m_configurator.Data->windowTitle, nullptr, m_configurator.Data->windowSize, m_configurator.Data->windowPos);
		m_frameBuffer = std::make_shared<FrameBuffer>(m_configurator.Data->framebufferSize.x, m_configurator.Data->framebufferSize.y);
		glfwMakeContextCurrent(GetWindow()->GetPointer());

		m_window->CloseWindowEvent.AddListener([&]() {
			this->Stop();
			});
		Im::Init(m_window->GetPointer());

		SetActiveProject(Project::OpenProject(testProjectPath));
		m_isRunning = true;
		m_time.Start();
		while (m_isRunning)
		{
			m_window->PollEvents();
			m_window->ClearWindow(Colors::blue);

			if (m_world)
				m_world->UpdateFrame(this);

			//m_uiManager.RenderUI();
			m_window->SwapBuffers();
		}

		Terminate();
	}

	void EditorApp::SetActiveWorld(SRef<World> world)
	{
		if (world == nullptr)
			m_world = std::make_shared<World>();
		else
			m_world = world;
	}
	void EditorApp::SetActiveProject(SRef<Project> newProject)
	{
		if (newProject == m_activeProject)
			return;

		m_activeProject = newProject;
		if (m_activeProject)
			m_window->SetTitle("Twisted Editor: " + newProject->GetName());

		//TODO:
		SetActiveWorld(nullptr);
	}
	bool EditorApp::Init()
	{
		if (Logger::Init() && RenderAPI::InitGLFW())
		{
			TWISTED_INFO("Application Init Success!");
			return true;
		}

		TWISTED_ERROR("Application Init failure");
		return false;
	}
	void EditorApp::Terminate()
	{
		RenderAPI::TerminateGLFW();
	}
	void EditorApp::LoadResources()
	{

		m_resources.LoadAssets(m_activeProject->GetAssetsFolder());
	}

	void EditorApp::SetSelectedEntity(EntityID selectedEntity)
	{
		if (m_selectedEntityID == selectedEntity)
			return;
		m_selectedEntityID = selectedEntity;
	}
}