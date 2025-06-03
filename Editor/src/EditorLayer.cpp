#include "editorpch.h"
#include "EditorLayer.h"

#include "Twisted/Application/Application.h"
#include "Twisted/Application/Window.h"

#include "UI/ImguiExtensions.h"
#include "UI/Panels/TreeViewPanel.h"
#include "UI/Panels/DetailsPanel.h"
#include "UI/Panels/WorldViewPanel.h"
#include "UI/NativeUtils.h"

#include "Utils/FileUtils.h"
#include "EditorConstants.h"

//#include "AppCore.h"
//#include "Twisted.h"
//
////#include "Rendering/RenderingAPI.h
//
//#include "EditorConfig.h"
//
//#include "Managers/PanelManager.h"
//

namespace Twisted::Editor
{
	void EditorLayer::OnInit()
	{
		//m_configurator.LoadConfig();
	}

	void EditorLayer::OnBeforeRun()
	{
		m_app->GetWindow()->SetTitle(Constants::windowTitle);

		Im::Init(m_app->GetWindow()->GetPointer());

		CreateEditorPanel<TreeViewPanel>();
		CreateEditorPanel<DetailsPanel>();
		//CreateEditorPanel<WorldViewPanel>();

		//SetActiveProject(Project::OpenProject("TODO"));//testProjectPath
	}

	void EditorLayer::OnFrame()
	{
		Im::StartFrame();

		RenderMenuBar();
		RenderDockSpace();

		Im::EndFrame();
	}

	void EditorLayer::SetActiveWorld(SRef<World> world)
	{
		if (world == nullptr)
			m_world = std::make_shared<World>(m_app);
		else
			m_world = world;
	}
	void EditorLayer::SetActiveProject(SRef<Project> newProject)
	{
		if (newProject == m_activeProject)
			return;

		m_activeProject = newProject;
		if (m_activeProject)
			m_app->GetWindow()->SetTitle(Constants::windowTitle + " : " + newProject->GetName());
		else
			m_app->GetWindow()->SetTitle(Constants::windowTitle);

		//TODO:
		SetActiveWorld(nullptr);
	}


	void EditorLayer::LoadResources()
	{
		//m_resources.LoadAssets(m_activeProject->GetAssetsFolder());
	}

	void EditorLayer::SetSelectedEntity(EntityID selectedEntity)
	{
		if (m_selectedEntityID == selectedEntity)
			return;
		m_selectedEntityID = selectedEntity;
	}

	void EditorLayer::RenderMenuBar()
	{
		// Push style settings
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 15));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
		auto var = m_app->GetWindow()->GetPointer();
		// Create the menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Project"))
				{
					while (true)
					{
						std::string path = Native::OpenFolderDialog(*m_app->GetWindow());
						if (path == "")
							break;
						if (!Utils::IsEmptyDirectory(path))
							continue;
						SetActiveProject(Project::CreateNewProject(path));
						break;
					}
				}
				if (ImGui::MenuItem("Open Project"))
				{
					std::vector<std::pair<std::wstring, std::wstring>> filter{ { L"project file (twisted.editor)",L"twisted.editor" } };
					std::string path = Native::OpenFileDialog(*m_app->GetWindow(), filter);
					if (path != "")
						SetActiveProject(Project::OpenProject(path));
				}
				if (ImGui::MenuItem("Exit"))
				{
					m_app->Stop();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Project"))
			{
				if (ImGui::MenuItem("New World"))
				{
					SetActiveWorld(std::make_shared<World>(m_app));
				}
				if (ImGui::MenuItem("Open World"))
				{
					std::string path = Native::OpenFileDialog(*m_app->GetWindow(), { {L"world file (*.world)",L"*.world"} });
					if (path != "")
					{
						BinSerializer buffer;
						buffer.LoadFromFile(path);
						m_world->Deserialize(buffer);
					}
				}
				if (ImGui::MenuItem("Save World As"))
				{
					if (m_world)
					{
						std::filesystem::path path = Native::SaveFileDialog(*m_app->GetWindow(), { {L"world file (*.world)",L"*.world"} });
						if (!path.empty())
						{
							BinSerializer buffer;
							m_world->Serialize(buffer);
							buffer.SaveToFile(path);
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
				if (ImGui::MenuItem("New World"))
				{
					SetActiveWorld(nullptr);
				}

				if (ImGui::MenuItem("New Entity"))
				{
					if (GetActiveWorld())
						EntityID newEntt = GetActiveWorld()->CreateNewEntity(NullEntity);
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

		ImGui::PopStyleVar(2); // Pop both FramePadding and ItemSpacing
	}

	void EditorLayer::RenderDockSpace()
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

}