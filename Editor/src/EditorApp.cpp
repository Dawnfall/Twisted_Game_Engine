#include "editorpch.h"
#include "EditorApp.h"
#include "Twisted/Rendering/RenderingAPI.h"

#include "UI/Editor_IMGUI.h"
#include "Debug/Logger.h"

#include "UI/Panels/DetailsPanel.h"
#include "UI/Panels/TreeViewPanel.h"

namespace Twisted::Editor
{
	void EditorApp::Run(Twisted::RuntimeBase* runtime)
	{
		Init();

		m_monitor = Monitor::CreateMonitorInfo();
		m_window = Window::CreateNewWindow(windowTitle, nullptr, windowSize, Vec2i(100, 100));

		glfwMakeContextCurrent(GetWindow()->GetPointer());
		m_window->CloseWindowEvent.AddListener([&]() {
			this->Stop();
			});

		InitIMGUI();

		m_isRunning = true;
		while (IsRunning())
		{
			m_window->PollEvents();
			RenderAPI::ClearWindow(Colors::blue);

			if (m_activeWorld)
				m_activeWorld->UpdateFrame(this);

			RenderUI();
			m_window->SwapBuffers();
		}

		Terminate();
	}

	bool EditorApp::Init()
	{
		if (Logger::Init() && RenderAPI::InitGLFW())
		{
			m_time.Init();
			TWISTED_INFO("Application Init Success!");
			return true;
		}
		else
		{
			TWISTED_ERROR("Application Init failure");
			return false;
		}
	}

	void EditorApp::Terminate()
	{
		RenderAPI::Terminate();
	}

	void EditorApp::LoadResources()
	{
		m_resources.LoadAssets(m_activeProject->GetAssetsFolder());
	}

	void EditorApp::InitIMGUI()
	{
		if (!glfwInit()) //due to globals and dlls glfw is not initialized outside of dll
		{
			TWISTED_ERROR("GLFW init failure; RenderCore Init failure!"); //TODO: editor output
		}

		ImguiAPI::Init(m_window->GetPointer());
		ImguiAPI::SetFlags();
		ImguiAPI::SetStyle();

		CreateEditorPanel<TreeViewPanel>();
		CreateEditorPanel<DetailsPanel>();
	}

	void EditorApp::RenderUI()
	{
		ImguiAPI::StartFrame();


		RenderMenuBar();
		RenderDockSpace();
		RenderModals();

		ImguiAPI::EndFrame();
	}

	void EditorApp::RenderDockSpace()
	{
		std::string mainDockSpaceLabel = "MainDockSpace";
		float menuBarHeight = ImGui::GetFrameHeight() + 22; // Get the actual height of the menu bar
		ImVec2 displaySize = ImGui::GetIO().DisplaySize;

		ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
		ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y - menuBarHeight));
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		ImGui::Begin(mainDockSpaceLabel.c_str(), nullptr, window_flags);
		ImGuiID dockspace_id = ImGui::GetID(mainDockSpaceLabel.c_str());
		ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_None);
		ImGui::End();

		for (auto& panel : m_panels)
		{
			ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
			ImGui::Begin(panel->GetName().c_str(), &panel->IsShowing);
			panel->RenderContent(this);
			ImGui::End();
		}
	}

	void EditorApp::RenderMenuBar()
	{
		// Push style settings
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 15));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));

		// Create the menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Project"))
				{
					if (!m_fileBrowser)
					{
						FileBrowserData data;
						data.Title = "Create New Project";
						data.BrowserType = BrowserType::DIRECTORY;
						data.IsMultiselect = false;
						data.StartDir = fs::current_path();

						m_fileBrowser = std::make_unique<FileBrowser>(data);
					}
				}
				if (ImGui::MenuItem("Open Project"))
				{
					if (m_fileBrowser)
					{
						FileBrowserData data;
						data.Title = "Open Project";
						data.BrowserType = BrowserType::DIRECTORY;
						data.IsMultiselect = false;
						data.StartDir = fs::current_path();

						m_fileBrowser = std::make_unique<FileBrowser>(data);
					}
				}
				if (ImGui::MenuItem("Exit"))
				{

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Panels"))
			{
				for (auto& window : m_panels)
				{
					if (ImGui::MenuItem(window->GetName().c_str(), nullptr, window->IsShowing))
						window->IsShowing = !window->IsShowing;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("New World"))
				{
					m_activeWorld = std::make_shared<World>();
				}

				if (ImGui::MenuItem("New Entity"))
				{
					if (m_activeWorld)
						EntityID newEntt = m_activeWorld->CreateNewEntity(NullEntity);
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

		// Pop style settings
		ImGui::PopStyleVar(2); // Pop both FramePadding and ItemSpacing
	}
	void EditorApp::RenderModals()
	{
		if (m_fileBrowser != nullptr)
		{
			if (!m_fileBrowser->Render())
			{
				std::vector<fs::path> results = m_fileBrowser->GetResults();
				if (results.size() == 1)
				{
					auto newProject = Project::CreateNewProject(results[0]);
					if (newProject)
						SetActiveProject(newProject);
				}
				m_fileBrowser = nullptr;
			}
		}
	}
}