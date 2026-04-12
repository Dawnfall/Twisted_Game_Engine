#include "UIWindow.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Windowing/NativeUtils.h"

//#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "EditorApp/EditorService.h"
#include "Twisted/Gameing/GameService.h"
#include "EditorApp/EditorRegistry.h"
#include "Twisted/Windowing/Window.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include "Utils/WPtr.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"
#include "Utils/GlmUtils.h"
#include "EditorConstants.h"
#include "Twisted/Gameing/World.h"
#include "Utils/WPtrBase.h"
#include <filesystem>
#include <utility>
#include <vector>
#include <imgui.h>
#include <string>

namespace Twisted::Editor
{
	void UIWindow::Render(Window* window)
	{
		Im::StartFrame();

		RenderMenuBar(window);
		RenderDockSpace();

		Im::EndFrame();
	}

	void UIWindow::RenderMenuBar(Window* window)
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
					std::filesystem::path selectedPath = Native::OpenFolderDialog(*window);
					if (selectedPath != "")
						SelectProject(selectedPath);
				}
				if (ImGui::MenuItem("Load Project"))
				{
					std::vector<std::pair<std::wstring, std::wstring>> filter = {
						{L"project file (twisted.editor)", L"twisted.editor"}
					};
					std::filesystem::path selectedPath = Native::OpenFileDialog(*window, filter);
					if (selectedPath != "")
						SelectProject(selectedPath);
				}
				if (ImGui::BeginMenu("Recent Projects"))
				{
					int count = 0;
					for (const std::string& recentProjPath : EditorConfig::GetInstance().GetRecentProjects())
					{
						if (count++ >= 5)
							break; // limit to 5 projects
						if (ImGui::MenuItem(recentProjPath.c_str()))
						{
							fs::path projPath = recentProjPath;
							SelectProject(projPath);
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Reload Assets"))
				{
					//assetsLayer->LoadAssets(*GetActiveProject());
				}
				if (ImGui::MenuItem("Exit"))
				{
					if (Native::ShowConfirmDialog(*window, L"Are you sure?", L"Exit editor?"))
						Application::GetInstance().GetService<EditorService>()->ConfirmedQuitEvent.Invoke();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Project"))
			{
				if (ImGui::MenuItem("New World"))
				{
					Application::GetInstance().GetService<GameService>()->NewGameWorld();
				}
				if (ImGui::MenuItem("Open World"))
				{
					if (fs::path path = Native::OpenFileDialog(*window, { {L"world file (*.world)",L"*.world"} }); !path.empty())
						Application::GetInstance().GetService<GameService>()->LoadWorld(path);
				}
				if (ImGui::MenuItem("Save World As"))
				{
					World* gameWorld = Application::GetInstance().GetService<GameService>()->GetGameWorld();
					if (gameWorld)
					{
						if (fs::path path = Native::SaveFileDialog(*window, { {L"world file (*.world)",L"*.world"} }); !path.empty())
						{
							path = path.replace_extension(".world");
							Application::GetInstance().GetService<AssetsService>()->SaveAssetDirect(path, { WPtr<World>(gameWorld) });
						}
					}
				}
				if (ImGui::MenuItem("Save World"))
				{
					auto* assetsService = Application::GetInstance().GetService<AssetsService>();
					World* world = Application::GetInstance().GetService<GameService>()->GetGameWorld();
					assetsService->Save(assetsService->GetObjectUuid(world));
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Panels"))
			{
				for (auto& panel : EditorRegistry::GetInstance().m_panels)
				{
					if (ImGui::MenuItem(panel->GetName().c_str(), nullptr, panel->IsShowing))
						panel->IsShowing = !panel->IsShowing;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Create"))
			{
				auto& importers = AssetImporterRegistry::GetInstance().GetImporters();

				for (auto imp : importers)
				{
					std::string createPath = imp->GetCreatePath();
					if (createPath != "")
						if (ImGui::MenuItem(createPath.c_str()))
							Application::GetInstance().GetService<EditorService>()->MakeNewFileEvent.Invoke(imp->DefaultFileName());
				}

				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
		ImGui::PopStyleVar(2); // Pop both FramePadding and ItemSpacing
	}

	void UIWindow::RenderDockSpace()
	{
		// Get viewport
		//const ImGuiViewport* viewport = ImGui::GetMainViewport();
		float menuBarHeight = ImGui::GetFrameHeight() + 22; // Get the actual height of the menu bar

		ImVec2 displaySize = ImGui::GetIO().DisplaySize;

		ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
		ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y - menuBarHeight));
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin(Constants::mainDockSpaceLabel.c_str(), nullptr, Constants::dockFlags);
		ImGui::PopStyleVar(2);

		//// Dockspace
		ImGuiID dockspace_id = ImGui::GetID(Constants::mainDockSpaceLabel.c_str());
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		ImGui::End();

		if (Application::GetInstance().GetService<AssetsService>()->GetProject().GetRootPath() != "")
		{
			for (auto& panel : EditorRegistry::GetInstance().m_panels)
			{
				if (!panel->IsShowing)
					continue;

				ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
				if (!panel->IsInit)
				{
					panel->IsInit = true;
				}

				ImGui::Begin(panel->GetName().c_str(), &panel->IsShowing, Constants::panelFlags);

				// Get the size of the panel
				ImVec2 size = ImGui::GetContentRegionAvail();

				// Resize the framebuffer if the size has changed
				if (size.x != panel->Size.x || size.y != panel->Size.y)
				{
					panel->Size = Vec2i(size.x, size.y);
					panel->PanelResizeEvent.Invoke();
				}

				panel->PaintContent();
				ImGui::End();
			}
		}
	}

	void UIWindow::SelectProject(fs::path& path)
	{
		if (fs::is_regular_file(path))
			path = path.parent_path();

		if (Application::GetInstance().GetService<AssetsService>()->SetProject(path))
			EditorConfig::GetInstance().AddLatest(path.string());
		else
			EditorConfig::GetInstance().RemoveEntry(path.string());
		EditorConfig::GetInstance().SaveConfig();
	}
}
