#include "ProjectLoader.h"
#include "EditorRuntime.h"

namespace Twisted::Editor
{
    ProjectLoader::ProjectLoader(EditorRuntime* editor) :m_editor(editor)
    {
        m_editor->GetWindow()->SetSize(Vec2i{ 1200,500 });

    }



    void ProjectLoader::Render()
    {
        if (!m_isInit)
            Init();
        else
        {
            ImGui::Begin(mainDockSpaceLabel.c_str(), nullptr, window_flags);
            if (ImGui::Button("New Project")) {
                std::filesystem::path
                    folder = Native::OpenFolderDialog(*m_editor->GetWindow());
                if (!folder.empty()) {
                    SelectedProject = Project::CreateNewProject(folder);
                }
            }

            if (ImGui::Button("Load Project")) {
                std::vector<std::pair<std::wstring, std::wstring>> filter = { {L"project file (twisted.editor)", L"twisted.editor"} };
                std::filesystem::path file = Native::OpenFileDialog(*m_editor->GetWindow(), filter);
                if (!file.empty()) {
                    SelectedProject = Project::OpenProject(file);
                }
            }
            ImGui::End();
        }     
    }

    void ProjectLoader::Init()
    {
        mainDockSpaceLabel = "MainDockSpace";
        float menuBarHeight = ImGui::GetFrameHeight() + 22; // Get the actual height of the menu bar
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y));
        ImGui::SetNextWindowBgAlpha(1.0f);
        window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        ImGui::Begin(mainDockSpaceLabel.c_str(), nullptr, window_flags);
        ImGuiID dockspace_id = ImGui::GetID(mainDockSpaceLabel.c_str());
        ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_None);
        ImGui::End();

        m_isInit = true;
    }
}