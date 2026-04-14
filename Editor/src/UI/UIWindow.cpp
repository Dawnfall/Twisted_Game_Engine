#include "UIWindow.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Windowing/WIN32/Win32Utils.h"

namespace Native = Twisted::Windows;

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

		RenderTitleBar(window);
		RenderMenuBar(window);
		RenderDockSpace();

		Im::EndFrame();
	}

	void UIWindow::RenderTitleBar(Window* window)
	{
		ImVec2 displaySize = ImGui::GetIO().DisplaySize;

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(displaySize.x, Constants::TITLE_BAR_HEIGHT));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 0.0f));

		constexpr ImGuiWindowFlags titleBarFlags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoMove;

		if (ImGui::Begin("##TitleBar", nullptr, titleBarFlags))
		{
			// Vertically center the title text
			float textY = (Constants::TITLE_BAR_HEIGHT - ImGui::GetTextLineHeight()) * 0.5f;
			ImGui::SetCursorPosY(textY);
			ImGui::Text("%s", Constants::EDITOR_WINDOW_TITLE.c_str());

			// Window control buttons (3 square buttons flush to the right)
			float btnSize = Constants::TITLE_BAR_HEIGHT;
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

			ImGui::SetCursorPos(ImVec2(displaySize.x - btnSize * 3, 0.0f));

			if (ImGui::Button("_", ImVec2(btnSize, btnSize)))
				window->Minimize();

			ImGui::SameLine();
			if (window->IsWindowMaximized())
			{
				if (ImGui::Button("[ ]", ImVec2(btnSize, btnSize)))
					window->Restore();
			}
			else
			{
				if (ImGui::Button("[ ]", ImVec2(btnSize, btnSize)))
					window->Maximize();
			}

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.1f, 0.1f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.65f, 0.0f, 0.0f, 1.0f));
			if (ImGui::Button("X", ImVec2(btnSize, btnSize)))
				window->CloseWindow();
			ImGui::PopStyleColor(2);

			ImGui::PopStyleVar(2);

			// Keep WM_NCHITTEST in sync with actual rendered height
			window->SetTitleBarHeight((int)Constants::TITLE_BAR_HEIGHT);
		}
		ImGui::End();
		ImGui::PopStyleVar(3);
	}

	void UIWindow::RenderMenuBar(Window* window)
	{
		ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		float menuBarHeight = ImGui::GetFontSize() + Constants::MENU_BAR_PADDING_Y * 2.0f;

		ImGui::SetNextWindowPos(ImVec2(0.0f, Constants::TITLE_BAR_HEIGHT));
		ImGui::SetNextWindowSize(ImVec2(displaySize.x, menuBarHeight));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,  ImVec2(15, Constants::MENU_BAR_PADDING_Y));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,   ImVec2(15, Constants::MENU_BAR_PADDING_Y));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		constexpr ImGuiWindowFlags menuWinFlags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_MenuBar;

		if (ImGui::Begin("##MenuBar", nullptr, menuWinFlags))
		{
			if (ImGui::BeginMenuBar())
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
			// --- Centered Play / Pause / Stop buttons ---
			{
				constexpr float btnW     = 60.0f;
				constexpr float btnH     = 0.0f;  // 0 = use default height
				constexpr float spacing  = 4.0f;
				constexpr float totalW   = btnW * 3 + spacing * 2;
				float centerX = (displaySize.x - totalW) * 0.5f;
				ImGui::SetCursorPosX(centerX);

				auto* gameService = Application::GetInstance().GetService<GameService>();
				GameState state   = gameService ? gameService->GetGameState() : GameState::Stopped;

				// Play button — disabled while already playing
				if (state == GameState::Playing)
				{
					ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
				}
				bool playDisabled = (state == GameState::Playing);
				if (playDisabled) ImGui::BeginDisabled();
				if (ImGui::Button("Play", ImVec2(btnW, btnH)) && gameService)
					gameService->Play();
				if (playDisabled) ImGui::EndDisabled();
				if (state == GameState::Playing) ImGui::PopStyleColor(3);

				ImGui::SameLine(0.0f, spacing);

				// Pause button — only active while playing
				bool pauseDisabled = (state != GameState::Playing);
				if (state == GameState::Paused)
				{
					ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.7f, 0.6f, 0.1f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.6f, 0.1f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.7f, 0.6f, 0.1f, 1.0f));
				}
				if (pauseDisabled) ImGui::BeginDisabled();
				if (ImGui::Button("Pause", ImVec2(btnW, btnH)) && gameService)
					gameService->Pause();
				if (pauseDisabled) ImGui::EndDisabled();
				if (state == GameState::Paused) ImGui::PopStyleColor(3);

				ImGui::SameLine(0.0f, spacing);

				// Stop button — disabled when already stopped
				bool stopDisabled = (state == GameState::Stopped);
				if (stopDisabled) ImGui::BeginDisabled();
				if (ImGui::Button("Stop", ImVec2(btnW, btnH)) && gameService)
					gameService->Stop();
				if (stopDisabled) ImGui::EndDisabled();
			}

			ImGui::EndMenuBar();
		}
		}
		ImGui::End();
		ImGui::PopStyleVar(5);
	}

	void UIWindow::RenderDockSpace()
	{
		ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		float menuBarHeight = ImGui::GetFontSize() + Constants::MENU_BAR_PADDING_Y * 2.0f;
		float topOffset = Constants::TITLE_BAR_HEIGHT + menuBarHeight;

		ImGui::SetNextWindowPos(ImVec2(0, topOffset));
		ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y - topOffset));
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
