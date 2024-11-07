#include "editorpch.h"
#include "EditorApp.h"
#include "Twisted/Rendering/RenderingAPI.h"
#include "Twisted/Windows/WindowAPI.h"

#include "UI/Editor_IMGUI.h"
#include "Debug/Logger.h"

#include "UI/Panels/DetailsPanel.h"
#include "UI/Panels/TreeViewPanel.h"
#include "imgui_internal.h"

namespace Twisted::Editor
{
	void EditorApp::Run()
	{
		if (m_runtime)
			m_runtime->OnBeforeRun(this);

		while (IsRunning())
		{
			WindowAPI::PollEvents();
			RenderAPI::ClearWindow(Colors::blue);

			m_world->UpdateFrame(this);

			RenderUI();
			WindowAPI::SwapBuffers(GetWindow());

			if (m_runtime)
				m_runtime->OnRun(this);
		}
	}



	void EditorApp::CreateNewWindow(const AppParams& params)
	{
		AppBase::CreateNewWindow(params);
		ImguiAPI::Init(m_window->Pointer);
		ImguiAPI::SetFlags();
		ImguiAPI::SetStyle();

		//app->closeWindowEvent.AddListener([app]() { app->Stop(); });

		CloseWindowEvent.AddListener([&]() {
			this->Stop();
			});

		CreateEditorPanel<TreeViewPanel>();
		CreateEditorPanel<DetailsPanel>();
	}

	void EditorApp::RenderUI()
	{
		ImguiAPI::StartFrame();
		RenderMenuBar();
		RenderDockSpace();

		ImguiAPI::EndFrame();
	}


	void EditorApp::RenderDockSpace()
	{
		std::string mainDockSpaceLabel = "MainDockSpace";
		float menuBarHeight = ImGui::GetFrameHeight();// +22; // Get the actual height of the menu bar
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
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 15));
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));

		// Create the menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New");
				ImGui::MenuItem("Open");
				ImGui::MenuItem("Save");
				ImGui::MenuItem("Exit");
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
				if (ImGui::MenuItem("New Entity"))
				{
					auto newEntt = m_world->CreateNewEntity();
				}
				ImGui::EndMenu();
			}

		}
		ImGui::EndMainMenuBar();

		// Pop style settings
		//ImGui::PopStyleVar(2); // Pop both FramePadding and ItemSpacing
	}
}