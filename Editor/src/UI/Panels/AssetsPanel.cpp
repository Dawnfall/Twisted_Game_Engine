#include "AssetsPanel.h"
#include "Twisted/Application/Application.h"
#include "EditorRegistry.h"
#include "UI/ImguiExtensions.h"

namespace Twisted::Editor
{
	AssetsPanel::AssetsPanel(EditorLayer* editor) :EditorPanel(editor,"Assets Panel")
	{
		Project::GetInstance().ProjectChangeEvent.AddListener([this]() {
			currentDir = Project::GetInstance().GetRootPath();
			});
	}

	void AssetsPanel::PaintContent()
	{
		float leftPanelWidth = 250.0f;
		float panelHeight = ImGui::GetContentRegionAvail().y;

		ImGui::Text("Current directory: %s", currentDir.string().c_str());

		// === LEFT PANEL: Folder Tree ===
		ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, panelHeight), true);
		//PaintTreePart(EditorRegistry::GetInstance().GetProject().GetRootPath());
		ImGui::EndChild();

		ImGui::SameLine();

		// === RIGHT PANEL: Folder Contents ===
		ImGui::BeginChild("RightPanel", ImVec2(0, panelHeight), true); // Width = 0 means fill remaining space

		bool isSelected;
		int columnCount = (int)(ImGui::GetContentRegionAvail().x / CELL_SIZE.x);

		std::filesystem::path selectedPath = "";

		if (columnCount > 0)
		{
			ImGui::Columns(columnCount, nullptr, false);
			for (const auto& entry : std::filesystem::directory_iterator(currentDir))
			{
				Im::AssetEntryToken token;
				token.Path = entry.path();
				token.IsSelected = m_selectedPath && m_selectedPath == entry.path();
				token.HighLightColor = HIGHLIGHT_COLOR;
				token.CellSize = CELL_SIZE;

				if (entry.path().extension() == ".info")
					continue;

				if (Im::AssetEntry(token))
					selectedPath = entry.path();
			}

			PaintNewAsset();
			ImGui::Columns(1);

			NewAssetPopup();
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
		ImGui::EndChild();

		if (selectedPath != "")
			m_selectedPath = selectedPath;
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
}