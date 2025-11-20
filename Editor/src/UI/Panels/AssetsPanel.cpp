#include "AssetsPanel.h"
#include "Twisted/Application/Application.h"
#include "EditorRegistry.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/AssetsLayer/AssetInfo.h"


namespace Twisted::Editor
{
	static ImGuiTreeNodeFlags GetAssetFlags(size_t childCount, const fs::path& assetPath)
	{
		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		if (childCount == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (EditorLayer::GetInstance()->GetSelection().GetSelectedPaths().contains(assetPath))
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		return flags;
	}

	static ImGuiTreeNodeFlags GetObjectFlags(TObject* obj)
	{
		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_Leaf |
			ImGuiTreeNodeFlags_NoTreePushOnOpen;

		if (EditorLayer::GetInstance()->GetSelection().GetSelectedObjects().contains(obj->GetID()))
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		return flags;
	}

	static void CheckLeftClickOnAsset(const fs::path& asset)
	{
		if (EditorLayer::GetInstance()->GetInput().IsClicked() && ImGui::IsItemHovered())//!ImGui::IsItemToggledOpen()
		{
			ImGuiIO& io = ImGui::GetIO();
			bool ctrlHeld = io.KeyCtrl; // true if Ctrl is held
			bool shiftHeld = io.KeyShift; // true if Shift is held
			bool altHeld = io.KeyAlt; // true if Alt is held

			if (ctrlHeld)
			{
				EditorLayer::GetInstance()->GetSelection().SelectPaths({ asset }, SelectionFlags::REMOVE_IF_SELECTED);
			}
			else if (shiftHeld)
			{
				//TODO:should select all visible from prev tto current
			}
			else
			{
				EditorLayer::GetInstance()->GetSelection().SelectPaths({ asset }, SelectionFlags::REMOVE_OTHERS);
			}
		}
	}

	static void CheckLeftClickOnObject(TObject* object)
	{
		if (EditorLayer::GetInstance()->GetInput().IsClicked() && ImGui::IsItemHovered())//!ImGui::IsItemToggledOpen()
		{
			ImGuiIO& io = ImGui::GetIO();
			bool ctrlHeld = io.KeyCtrl; // true if Ctrl is held
			bool shiftHeld = io.KeyShift; // true if Shift is held
			bool altHeld = io.KeyAlt; // true if Alt is held

			if (ctrlHeld)
			{
				EditorLayer::GetInstance()->GetSelection().SelectObject({ object }, SelectionFlags::REMOVE_IF_SELECTED);
			}
			else if (shiftHeld)
			{
				//TODO:should select all visible from prev tto current
			}
			else
			{
				EditorLayer::GetInstance()->GetSelection().SelectObject({ object }, SelectionFlags::REMOVE_OTHERS);
			}
		}
	}

	void AssetsPanel::Init()
	{
		AssetsLayer::GetInstance()->GetProject().ProjectChangeEvent.AddListener([this]() {
			currentDir = AssetsLayer::GetInstance()->GetProject().GetAssetsFolder();
			});
		EditorLayer::GetInstance()->MakeNewFileEvent.AddListener([this](fs::path defaultName) {
			m_newFileName = Im::InputTextToken{};
			m_newFileName.value().PostLabel = defaultName.extension().string();
			m_newFileName.value().Text = defaultName.stem().string();
			m_newFileName.value().DoAutoFocus = true;
			});
	}

	void AssetsPanel::PaintContent()
	{
		float leftPanelWidth = 250.0f;
		float panelHeight = ImGui::GetContentRegionAvail().y;

		ImGui::Text("Current directory: %s", currentDir.string().c_str());

		// LEFT PANEL: Folder Tree
		ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, panelHeight), true);
		PaintTreePart(AssetsLayer::GetInstance()->GetProject().GetRootPath());
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

		NewAssetPopup();
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
		bool justSelected = false;
		fs::path filename = assetPath.filename();

		AssetsLayer* assetsLayer = AssetsLayer::GetInstance();
		AssetInfo* info = assetsLayer->GetInfo(assetPath);
		auto& objects = assetsLayer->GetManagedAssetObjects(info);

		ImGuiTreeNodeFlags flags = GetAssetFlags(objects.size(), (info) ? assetPath : "");

		// tree node
		bool isOpened = ImGui::TreeNodeEx(filename.string().c_str(), flags);

		CheckLeftClickOnAsset(assetPath);

		// drag source
		std::string pathAsString = filename.string();
		Im::DragSource<AssetInfo*>(Constants::ASSET_DRAG_TYPE, info, filename.string().c_str());

		if (isOpened)
		{
			for (auto& obj : objects)
			{
				TObject* objPtr = obj.GetObj();
				std::string name = assetPath.stem().string();

				name += "##" + std::to_string(objPtr->GetID().GetID());
				ImGuiTreeNodeFlags objFlags = GetObjectFlags(objPtr);
				ImGui::TreeNodeEx(name.c_str(), objFlags);
				CheckLeftClickOnObject(objPtr);
				Im::DragSource<TObject*>(Constants::OBJECT_DRAG_TYPE, objPtr, "Object");
			}
			ImGui::TreePop();
		}
		return justSelected;
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
					AssetsLayer::GetInstance()->CreateNewAsset(path);
				}
				m_newFileName = std::nullopt;
			}
		}
	}

	void AssetsPanel::NewAssetPopup()
	{
		if (ImGui::BeginPopupContextWindow("AssetPopup", ImGuiPopupFlags_MouseButtonRight))
		{
			for (auto& importer : AssetImporterRegistry::GetInstance().GetImporters())
			{
				std::string createPath = importer->GetCreatePath();
				if (createPath != "")
				{
					if (ImGui::MenuItem(createPath.c_str()))
					{
						EditorLayer::GetInstance()->MakeNewFileEvent.Invoke(importer->DefaultFileName());
					}
				}
			}
			ImGui::EndPopup();
		}
	}
}

REGISTER_EDITOR_PANEL(AssetsPanel)
