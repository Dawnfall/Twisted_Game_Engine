#include "editorpch.h"
#include "UIManager.h"
#include "EditorApp.h"
#include "UI/FileBrowser/FileBrowser.h"
#include "AppCore.h"

#include "ImguiExtensions.h"
#include "Twisted/Game/World.h"

namespace Twisted::Editor
{
	void UIManager::RenderUI()
	{
		Im::StartFrame();

		RenderMenuBar();
		RenderDockSpace();
		RenderModals();

		Im::EndFrame();
	}

	void UIManager::RenderMenuBar()
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
					Editor->SetActiveWorld(nullptr);
				}

				if (ImGui::MenuItem("New Entity"))
				{
					if (Editor->GetActiveWorld())
						EntityID newEntt = Editor->GetActiveWorld()->CreateNewEntity(NullEntity);
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

		// Pop style settings
		ImGui::PopStyleVar(2); // Pop both FramePadding and ItemSpacing
	}

	void UIManager::RenderDockSpace()
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
				panel->Size = size;
				panel->PanelResizeEvent.Invoke();
			}

			panel->RenderContent(Editor);
			ImGui::End();
		}
	}

	void UIManager::RenderModals()
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
						Editor->SetActiveProject(newProject);
				}
				m_fileBrowser = nullptr;
			}
		}
	}

}