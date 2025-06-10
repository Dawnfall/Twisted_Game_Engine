#include "EditorRuntime.h"
#include "imgui.h"
#include "Windowing/NativeUtils.h"
#include "Utils/FileUtils.h"

#include "EditorConstants.h"
#include "UI/Panels/DetailsPanel.h"
#include "UI/Panels/TreeViewPanel.h"
#include "UI/Panels/WorldViewPanel.h"
#include "UI/ImguiExtensions.h"

namespace Twisted::Editor
{
	void EditorRuntime::OnCreate()
	{
		m_windowLayer = App->AddLayer<Twisted::WindowLayer>();
		m_renderLayer = App->AddLayer<Twisted::Render::RenderLayer>();
		m_gameLayer = App->AddLayer<Twisted::GameLayer>();
	}

	void EditorRuntime::OnInit()
	{
		//ProjectChangeEvent.AddListener([this]() {
		//	if (GetActiveProject())
		//		m_windowLayer->GetWindow()->SetTitle(Constants::windowTitle + " : " + GetActiveProject()->GetName());
		//	else
		//		m_windowLayer->GetWindow()->SetTitle(Constants::windowTitle);
		//	});

		m_window = m_windowLayer->CreateNewWindow("Temp name", Vec2i(1000, 800), Vec2i(400, 400));
		rend::InitRenderer(m_window->GetContextAdress());
		Im::Init(m_windowLayer->GetWindow()->GetRawPointer());

		m_window->CloseWindowEvent.AddListener([&]() {
			this->App->Stop();
			});
		m_window->SetTitle(Constants::windowTitle);

		CreateEditorPanel<TreeViewPanel>();
		CreateEditorPanel<DetailsPanel>();
		CreateEditorPanel<WorldViewPanel>();
		//m_configurator.LoadConfig();
	}

	void EditorRuntime::OnBeforeRun()
	{
		m_gameWorld = m_gameLayer->CreateNewWorld();
		m_editorWorld = m_gameLayer->CreateNewWorld();

		m_projectLoader = std::make_unique<ProjectLoader>(this);
	}

	void EditorRuntime::OnFrame()
	{
		m_window->PollEvents();
		//m_window->ClearWindow();

		Im::StartFrame();

		if (m_projectLoader)
		{

			//ImGui::Begin("Project Loader", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar);

			m_projectLoader->Render();
			if (m_projectLoader->SelectedProject)
			{
				SetActiveProject(m_projectLoader->SelectedProject);
				m_projectLoader = nullptr;
			}

			//ImGui::End();
		}
		else
		{
			RenderMenuBar();
			RenderDockSpace();
		}

		Im::EndFrame();
		m_window->SwapBuffers();
	}

	void EditorRuntime::SetSelectedEntity(EntityID selectedEntity)
	{
		if (m_selectedEntityID == selectedEntity)
			return;
		m_selectedEntityID = selectedEntity;
	}

	void EditorRuntime::RenderMenuBar()
	{
		// Push style settings
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 15));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
		// Create the menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					App->Stop();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Project"))
			{
				if (ImGui::MenuItem("New World"))
				{
					CreateNewWorld();
				}
				if (ImGui::MenuItem("Open World"))
				{
					std::filesystem::path path = Native::OpenFileDialog(*m_windowLayer->GetWindow(), { {L"world file (*.world)",L"*.world"} });
					if (!path.empty())
					{
						LoadWorld(path);
					}
				}
				if (ImGui::MenuItem("Save World As"))
				{
					if (m_gameWorld)
					{
						std::filesystem::path path = Native::SaveFileDialog(*m_windowLayer->GetWindow(), { {L"world file (*.world)",L"*.world"} });
						if (!path.empty())
						{
							SaveWorldAs(path);
						}
					}
				}
				if (ImGui::MenuItem("Save World"))
				{

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Panels"))
			{
				for (auto& panel : Panels)
				{
					if (ImGui::MenuItem(panel->GetName().c_str(), nullptr, panel->IsShowing))
						panel->IsShowing = !panel->IsShowing;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("New Entity"))
				{
					if (m_gameWorld)
						EntityID newEntt = m_gameWorld->CreateEntity();
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

		ImGui::PopStyleVar(2); // Pop both FramePadding and ItemSpacing
	}

	void EditorRuntime::RenderDockSpace()
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

		for (auto& panel : Panels)
		{
			ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
			ImGui::Begin(panel->GetName().c_str(), &panel->IsShowing);

			// Get the size of the panel
			ImVec2 size = ImGui::GetContentRegionAvail();

			// Resize the framebuffer if the size has changed
			if (size.x != panel->Size.x || size.y != panel->Size.y)
			{
				panel->Size = Vec2i(size.x, size.y);
				panel->PanelResizeEvent.Invoke();
			}

			panel->RenderContent();
			ImGui::End();
		}
	}

	void EditorRuntime::CreateNewWorld()
	{
		if (m_gameWorld)
		{
			m_gameLayer->DestroyWorld(m_gameWorld);
			m_gameLayer->DestroyWorld(m_editorWorld);
		}
		m_gameWorld = m_gameLayer->CreateNewWorld();
		m_editorWorld = m_gameLayer->CreateNewWorld();

		WorldChangeEvent.Invoke();
	}

	void EditorRuntime::LoadWorld(const std::filesystem::path& path)
	{
		BinSerializer buffer;
		buffer.LoadFromFile(path);
		m_gameWorld->Deserialize(buffer);
	}

	void EditorRuntime::SaveWorldAs(const std::filesystem::path& path)
	{
		BinSerializer buffer;
		m_gameWorld->Serialize(buffer);
		buffer.SaveToFile(path);
	}
}