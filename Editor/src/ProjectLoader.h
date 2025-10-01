#pragma once
#include "Logger.h"
#include "Twisted/Data/Project.h"
#include "Twisted/Application/Layer.h"
#include "Constants.h"

#include "LoadupConfig.h"
#include "Twisted/Windowing/NativeUtils.h"
#include "Twisted/Windowing/Window.h"

#include <imgui.h>
#include <string>
#include <vector>

namespace Twisted::Editor
{
	class ProjectLoader :public Layer
	{

	public:
		ProjectLoader(Application* app) :
			Layer(app)
		{
		}


		void Init()override
		{
			LoadupConfigData.File.Load();
		}

		void Render(Window* window)
		{
			ImVec2 displaySize = ImGui::GetIO().DisplaySize;

			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(displaySize);
			ImGui::SetNextWindowBgAlpha(1.0f);

			ImGuiWindowFlags flags =
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse;

			if (ImGui::Begin("Project Picker", nullptr, flags))
			{
				if (ImGui::BeginTable("ProjectPickerMain", 2, ImGuiTableFlags_SizingStretchProp))
				{
					// Left column: recent projects
					ImGui::TableNextColumn();
					ImGui::Text("Recent:");
					ImGui::Separator();
					int count = 0;
					for (const std::string& recentProjPath : LoadupConfigData.GetRecentProjects())
					{
						if (count++ >= 5) break; // limit to 5 projects
						if (ImGui::Button(recentProjPath.c_str(), ImVec2(-FLT_MIN, 0)))
						{
							std::filesystem::path selectedPath = recentProjPath;
							if (fs::is_regular_file(selectedPath))
								selectedPath = selectedPath.parent_path();

							SelectProjectPath(selectedPath);
						}
					}

					// Right column: action buttons
					ImGui::TableNextColumn();
					if (ImGui::Button("New Project", ImVec2(-FLT_MIN, 0)))
					{
						std::filesystem::path selectedPath = Native::OpenFolderDialog(*window);
						if (selectedPath != "")
							SelectProjectPath(selectedPath);
					}

					if (ImGui::Button("Load Project", ImVec2(-FLT_MIN, 0)))
					{
						std::vector<std::pair<std::wstring, std::wstring>> filter = {
							{L"project file (twisted.editor)", L"twisted.editor"}
						};
						std::filesystem::path selectedPath = Native::OpenFileDialog(*window, filter);
						if (selectedPath != "")
							SelectProjectPath(selectedPath);
					}

					ImGui::EndTable();
				}
			}
			ImGui::End();
		}

	private:
		LoadupConfig LoadupConfigData;


		void SelectProjectPath(const fs::path& projectFolder)
		{
			if (!Utils::IsExisting(projectFolder))
			{
				TWISTED_WARN(std::format("projectFolder must exist {}", projectFolder.string()));
				LoadupConfigData.RemoveEntry(projectFolder.string());
				return;
			}
			if (Utils::IsEmptyDirectory(projectFolder) || !Utils::IsExisting(projectFolder / ProjectFile))
			{
				TWISTED_WARN("Project path must be empty directory or have twisted.editor file: " + projectFolder.string());
				LoadupConfigData.RemoveEntry(projectFolder.string());
				return;
			}

			LoadupConfigData.AddLatest(projectFolder.string());
			LoadupConfigData.AddLatest(projectFolder.string());
			LoadupConfigData.File.Save();

			Project::GetInstance().SetProject(projectFolder);
		}
	};
}

