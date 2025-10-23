#include "AssetsPanel.h"
#include "Twisted/Application/Application.h"
#include "EditorRegistry.h"
#include "UI/ImguiExtensions.h"
#include "EditorMacros.h"
#include "EditorData/EditorData.h"
#include "Twisted/AssetsLayer/AssetInfo.h"

namespace Twisted::Editor
{
	AssetsPanel::AssetsPanel() :EditorPanel("Assets Panel")
	{
		Project::GetInstance().ProjectChangeEvent.AddListener([this]() {
			currentDir = Project::GetInstance().GetAssetsFolder();
			});
	}

	void AssetsPanel::PaintContent()
	{
		float leftPanelWidth = 250.0f;
		float panelHeight = ImGui::GetContentRegionAvail().y;

		ImGui::Text("Current directory: %s", currentDir.string().c_str());

		// LEFT PANEL: Folder Tree
		ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, panelHeight), true);
		PaintTreePart(Project::GetInstance().GetRootPath());
		ImGui::EndChild();

		ImGui::SameLine();

		// RIGHT PANEL: Folder Contents
		ImGui::BeginChild("RightPanel", ImVec2(0, panelHeight), true); // Width = 0 means fill remaining space

		for (const auto& entry : std::filesystem::directory_iterator(currentDir))
		{
			if (!entry.exists())
				continue;
			if (!entry.is_regular_file())
				continue;
			if (entry.path().extension() == ".info")
				continue;

			if (AssetEntry(entry.path()))
			{

			}
		}
		PaintNewAsset();

		ImGui::EndChild();

	}


	void AssetsPanel::PaintTreePart(const fs::path& dirPath)
	{
		for (const auto& entry : fs::directory_iterator(dirPath)) {
			if (!entry.is_directory()) continue;

			const fs::path& folderPath = entry.path();
			const std::string folderName = folderPath.filename().string();
			const std::string fullPath = folderPath.string();

			// Tree node flags
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			if (currentDir == fullPath)
				flags |= ImGuiTreeNodeFlags_Selected;

			// Tree node UI
			bool open = ImGui::TreeNodeEx(folderName.c_str(), flags);

			if (ImGui::IsItemClicked()) {
				currentDir = fullPath; // Update selected folder
			}

			if (open) {
				PaintTreePart(folderPath); // Recursively render subfolders
				ImGui::TreePop();
			}
		}
	}


	bool AssetsPanel::AssetEntry(const fs::path& assetPath)
	{
		bool isSelected = EditorData::GetInstance().GetSelection().GetSelectedPaths().contains(assetPath);

		DragPayload& dragPayload = EditorData::GetInstance().GetDragPayload();

		AssetEntryToken token;
		token.Path = assetPath;
		token.IsSelected = m_selectedPath && m_selectedPath == assetPath;
		token.HighLightColor = HIGHLIGHT_COLOR;
		token.CellSize = CELL_SIZE;

		bool justSelected = false;

		std::string filename = assetPath.filename().string();
		std::string fullPath = token.Path.string();

		if (ImGui::TreeNodeEx(filename.c_str()))
		{
			if (ImGui::BeginDragDropSource())
			{
				if (!dragPayload.m_data.has_value())
				{
					SRef<AssetInfo> assetInfo = AssetsRegistry::GetInstance().GetInfo(filename);
					dragPayload.m_data = assetInfo;
				}

				ImGui::Text(filename.c_str());
				ImGui::EndDragDropSource();
			}

			auto& objects = AssetsRegistry::GetInstance().GetAssetObjects(assetPath);
			for (auto& obj : objects)
			{
				std::string name = assetPath.stem().string();
				name += "##" + std::to_string(obj.second.GetID().GetID());
				ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
			}

			ImGui::TreePop();
		}

		return justSelected;
	} //TODO... use later

	void AssetsPanel::PaintNewAsset()
	{
		if (m_newFileName.has_value())
		{
			Im::InputText(m_newFileName.value());
			if (m_newFileName->IsLostFocus)
			{
				std::filesystem::path path = currentDir / (m_newFileName->Text + m_newFileName->PostLabel);
				if (!path.empty() && path.has_filename() && !std::filesystem::exists(path))
				{
					if (path.extension().empty())
						std::filesystem::create_directory(path);
					else
						std::ofstream ofs(path.string());
				}
				m_newFileName = std::nullopt;
			}
		}
	}

	void AssetsPanel::NewAssetPopup()
	{
		auto paintMenuItem = [this](const std::string& menuItemText, const std::string& extension, const std::string& defaultFileName) {
			if (ImGui::MenuItem(menuItemText.c_str()))
			{
				Im::InputTextToken token;
				token.Text = defaultFileName;
				token.PostLabel = extension;
				m_newFileName = token;
			}
			};

		if (ImGui::BeginPopupContextWindow("AssetPopup", ImGuiPopupFlags_MouseButtonRight))
		{
			paintMenuItem("New Material", ".material", "New Material");
			paintMenuItem("New Shader", ".shader", "New Shader");
			paintMenuItem("New Folder", "", "New Folder");

			ImGui::EndPopup();
		}
	}

}

REGISTER_EDITOR_PANEL(AssetsPanel)
