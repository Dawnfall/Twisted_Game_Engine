#include "EditorLayer.h"
#include "Twisted/Application/Application.h"
#include "Twisted/AssetsLayer/Project.h"
#include "Twisted/Windowing/NativeUtils.h"
#include "UI/ImguiExtensions.h"


#include "AppCore.h"
#include "Twisted/Windowing/Window.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"

#include "Twisted/Gameing/World.h"
#include "Twisted/AssetsLayer/Importers/WorldImporter.h"
#include "Twisted/Gameing/WorldSerializer.h"

#include <imgui.h>
#include <string>

#include "UI/Panels/DetailsPanel.h"
#include "UI/Panels/TreeViewPanel.h"
#include "UI/Panels/WorldViewPanel.h"
#include "UI/Panels/AssetsPanel.h"

#include "Twisted/Gameing/Entity.h"
#include "EditorRegistry.h"
#include "EditorData/EditorData.h"

namespace Twisted::Editor
{
	void EditorLayer::Init()
	{
		EditorData::GetInstance().GameViewBuffer = TObject::Create<FrameBuffer>();
		EditorData::GetInstance().EditorViewBuffer = TObject::Create<FrameBuffer>();
		
		EditorData::GetInstance().SetWorld(nullptr);
	}



	void EditorLayer::Render(Window* window)
	{
		RenderMenuBar(window);
		RenderDockSpace();
	}

	void EditorLayer::RenderMenuBar(Window* window)
	{
		AssetsLayer* assetsLayer = m_app->GetLayer<AssetsLayer>();

		// Push style settings
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 15));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
		// Create the menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				//if (ImGui::MenuItem("Reload Assets"))
				//{
					//assetsLayer->LoadAssets(*GetActiveProject());
				//}
				if (ImGui::MenuItem("Exit"))
				{
					if (Native::ShowConfirmDialog(*window, L"Are you sure?", L"Exit editor?"))
						GetApplication()->Stop();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Project"))
			{
				if (ImGui::MenuItem("New World"))
				{
					EditorData::GetInstance().SetWorld(nullptr);
				}
				if (ImGui::MenuItem("Open World"))
				{
					std::filesystem::path path = Native::OpenFileDialog(*window, { {L"world file (*.world)",L"*.world"} });
					if (!path.empty())
					{
						World* world = assetsLayer->ImportAssetDirect<World>(path);
						if (world)
							EditorData::GetInstance().SetWorld(world);
					}
				}
				if (ImGui::MenuItem("Save World As"))
				{
					World* gameWorld = EditorData::GetInstance().GetGameWorld();
					if (gameWorld)
					{
						std::filesystem::path path = Native::SaveFileDialog(*window, { {L"world file (*.world)",L"*.world"} });
						path = path.replace_extension(".world");

						BinSerializer serBuffer = WorldSerializer::GetInstance().Serialize(*gameWorld);
						serBuffer.SaveToFile(path);
					}
				}
				if (ImGui::MenuItem("Save World"))
				{
					//TODO:... since world isnt managed by assetsLayer we need to track in manually
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
				if (ImGui::MenuItem("New Entity"))
				{
					if (EditorData::GetInstance().GetGameWorld())
						Entity newEntity = EditorData::GetInstance().GetGameWorld()->CreateNew<>();
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

		ImGui::PopStyleVar(2); // Pop both FramePadding and ItemSpacing
	}


	void EditorLayer::RenderDockSpace()
	{
		// Get viewport
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		float menuBarHeight = ImGui::GetFrameHeight() + 22; // Get the actual height of the menu bar

		ImVec2 displaySize = ImGui::GetIO().DisplaySize;

		ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
		ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y - menuBarHeight));
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin(mainDockSpaceLabel.c_str(), nullptr, dockFlags);
		ImGui::PopStyleVar(2);

		//// Dockspace
		ImGuiID dockspace_id = ImGui::GetID(mainDockSpaceLabel.c_str());
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		ImGui::End();

		for (auto& panel : EditorRegistry::GetInstance().m_panels)
		{
			if (!panel->IsShowing)
				continue;

			ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
			if (!panel->IsInit)
			{
				panel->IsInit = true;
			}

			ImGui::Begin(panel->GetName().c_str(), &panel->IsShowing, panelFlags);

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