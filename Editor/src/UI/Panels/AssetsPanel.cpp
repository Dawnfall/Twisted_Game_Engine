#include "UI/Panels/AssetsPanel.h"
#include "EditorApp/EditorRegistry.h"
#include "UI/ImguiExtensions.h"
#include "ImportedAsset.h"
#include "AssetImporterRegistry.h"
#include "Application/TObject.h"
#include "EditorConstants.h"
#include "EditorApp/EditorService.h"
#include "Input.h"
#include "Project.h"
#include <optional>
#include <filesystem>
#include <string>
#include "AssetsService.h"
#include <EditorData/Selection.h>
#include <imgui.h>


namespace Twisted::Editor
{
	static ImGuiTreeNodeFlags GetFileFlags(const fs::path& assetPath)
	{
		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_Leaf |
			ImGuiTreeNodeFlags_NoTreePushOnOpen |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		if (Application::GetInstance().GetService<EditorService>()->GetSelection().GetSelectedPaths().contains(assetPath))
			flags |= ImGuiTreeNodeFlags_Selected;

		return flags;
	}

	static ImGuiTreeNodeFlags GetObjectFlags(TObject* obj)
	{
		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_Leaf |
			ImGuiTreeNodeFlags_NoTreePushOnOpen;

		if (Application::GetInstance().GetService<EditorService>()->GetSelection().GetSelectedObjects().contains(obj->GetID()))
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		return flags;
	}

	static void CheckLeftClickOnAsset(const fs::path& asset)
	{
		if (Input::GetInstance().IsClicked() && ImGui::IsItemHovered())//!ImGui::IsItemToggledOpen()
		{
			ImGuiIO& io = ImGui::GetIO();
			bool ctrlHeld = io.KeyCtrl; // true if Ctrl is held
			bool shiftHeld = io.KeyShift; // true if Shift is held
			//bool altHeld = io.KeyAlt; // true if Alt is held

			if (ctrlHeld)
			{
				Application::GetInstance().GetService<EditorService>()->GetSelection().SelectPaths({ asset }, SelectionFlags::REMOVE_IF_SELECTED);
			}
			else if (shiftHeld)
			{
				//TODO:should select all visible from prev tto current
			}
			else
			{
				Application::GetInstance().GetService<EditorService>()->GetSelection().SelectPaths({ asset }, SelectionFlags::REMOVE_OTHERS);
			}
		}
	}

	static void CheckLeftClickOnObject(TObject* object)
	{
		if (Input::GetInstance().IsClicked() && ImGui::IsItemHovered())//!ImGui::IsItemToggledOpen()
		{
			ImGuiIO& io = ImGui::GetIO();
			bool ctrlHeld = io.KeyCtrl; // true if Ctrl is held
			bool shiftHeld = io.KeyShift; // true if Shift is held
			//bool altHeld = io.KeyAlt; // true if Alt is held

			if (ctrlHeld)
			{
				Application::GetInstance().GetService<EditorService>()->GetSelection().SelectObject({ object }, SelectionFlags::REMOVE_IF_SELECTED);
			}
			else if (shiftHeld)
			{
				//TODO:should select all visible from prev tto current
			}
			else
			{
				Application::GetInstance().GetService<EditorService>()->GetSelection().SelectObject({ object }, SelectionFlags::REMOVE_OTHERS);
			}
		}
	}

	void AssetsPanel::Init()
	{
		Twisted::Application::GetInstance().GetService<AssetsService>()->ProjectChangeEvent.AddListener([this]( const Project& newProject) {
			currentDir = newProject.GetAssetsFolder();
			});
		Application::GetInstance().GetService<EditorService>()->MakeNewFileEvent.AddListener([this](fs::path defaultName) {
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

		// LEFT PANEL: Folder list + folder creation
		ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, panelHeight), true);
		PaintTreePart(currentDir);
		PaintNewFolder();
		if (ImGui::BeginPopupContextWindow("FolderPopup", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("New Folder"))
			{
				m_newFolderName = Im::InputTextToken{};
				m_newFolderName->Text = "NewFolder";
				m_newFolderName->DoAutoFocus = true;
			}
			ImGui::EndPopup();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		// RIGHT PANEL: Asset files only
		ImGui::BeginChild("RightPanel", ImVec2(0, panelHeight), true);

		// Built-in assets
		if (ImGui::CollapsingHeader("Built-in"))
		{
			AssetsService* assets = Application::GetInstance().GetService<AssetsService>();
			for (TObject* obj : assets->GetBuiltInObjectsOfType())
			{
				std::string label = obj->GetName() + "##builtin_" + std::to_string(obj->GetID().GetID());
				ImGui::TreeNodeEx(label.c_str(), GetObjectFlags(obj));
				CheckLeftClickOnObject(obj);
				Im::DragSource<TObject*>(Constants::OBJECT_DRAG_TYPE, obj, obj->GetName().c_str());
			}
		}

		ImGui::Separator();

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

		if (m_pendingDelete.has_value())
		{
			Application::GetInstance().GetService<AssetsService>()->DeleteAsset(*m_pendingDelete);
			m_pendingDelete = std::nullopt;
		}

		NewAssetPopup();
		PaintNewAsset();

		ImGui::EndChild();
	}

	void AssetsPanel::PaintTreePart(const fs::path& dirPath)
	{
		const fs::path assetsRoot = Application::GetInstance().GetService<AssetsService>()->GetProject().GetAssetsFolder();

		if (currentDir != assetsRoot)
		{
			ImGuiTreeNodeFlags backFlags =
				ImGuiTreeNodeFlags_Leaf |
				ImGuiTreeNodeFlags_NoTreePushOnOpen |
				ImGuiTreeNodeFlags_SpanAvailWidth;
			ImGui::TreeNodeEx("..##back", backFlags);
			if (Input::GetInstance().IsClicked() && ImGui::IsItemHovered())
				currentDir = currentDir.parent_path();
		}

		for (const auto& entry : fs::directory_iterator(dirPath))
		{
			if (!entry.is_directory()) continue;

			const fs::path& folderPath = entry.path();
			std::string label = folderPath.filename().string() + "##" + folderPath.string();

			ImGuiTreeNodeFlags flags =
				ImGuiTreeNodeFlags_Leaf |
				ImGuiTreeNodeFlags_NoTreePushOnOpen |
				ImGuiTreeNodeFlags_SpanAvailWidth;

			ImGui::TreeNodeEx(label.c_str(), flags);

			if (Input::GetInstance().IsClicked() && ImGui::IsItemHovered())
				currentDir = folderPath;
		}
	}

	bool AssetsPanel::AssetEntry(const fs::path& assetPath)
	{
		fs::path filename = assetPath.filename();

		AssetsService* assetsLayer = Application::GetInstance().GetService<AssetsService>();
		ImportedAsset* info = assetsLayer->GetInfo(assetPath);

		if (!info)
		{
			ImGui::TreeNodeEx(filename.string().c_str(), GetFileFlags(assetPath));
			CheckLeftClickOnAsset(assetPath);
			return false;
		}

		ImGuiTreeNodeFlags rootFlags =
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_SpanAvailWidth;
		if (Application::GetInstance().GetService<EditorService>()->GetSelection().GetSelectedPaths().contains(assetPath))
			rootFlags |= ImGuiTreeNodeFlags_Selected;

		std::string rootLabel = info->GetAssetName() + "##asset_" + assetPath.string();
		bool isOpen = ImGui::TreeNodeEx(rootLabel.c_str(), rootFlags);
		CheckLeftClickOnAsset(assetPath);
		Im::DragSource<ImportedAsset*>(Constants::ASSET_DRAG_TYPE, info, info->GetAssetName().c_str());

		std::string ctxId = "asset_ctx_" + assetPath.string();
		if (ImGui::BeginPopupContextItem(ctxId.c_str()))
		{
			if (ImGui::MenuItem("Delete"))
				m_pendingDelete = assetPath;
			ImGui::EndPopup();
		}

		if (isOpen)
		{
			std::string fileLabel = filename.string() + "##file_" + assetPath.string();
			ImGui::TreeNodeEx(fileLabel.c_str(), GetFileFlags(assetPath));
			CheckLeftClickOnAsset(assetPath);
			Im::DragSource<ImportedAsset*>(Constants::ASSET_DRAG_TYPE, info, filename.string().c_str());
			ImGui::TreePop();
		}
		return false;
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
					Application::GetInstance().GetService<AssetsService>()->CreateNewAsset(path);
				}
				m_newFileName = std::nullopt;
			}
		}
	}

	void AssetsPanel::NewAssetPopup()
	{
		if (ImGui::BeginPopupContextWindow("AssetPopup", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			for (auto& importer : AssetImporterRegistry::GetInstance().GetImporters())
			{
				std::string createPath = importer->GetCreatePath();
				if (createPath != "")
				{
					if (ImGui::MenuItem(createPath.c_str()))
					{
						Application::GetInstance().GetService<EditorService>()->MakeNewFileEvent.Invoke(importer->DefaultFileName());
					}
				}
			}
			ImGui::EndPopup();
		}
	}

	void AssetsPanel::PaintNewFolder()
	{
		if (m_newFolderName.has_value())
		{
			Im::InputText(m_newFolderName.value());
			if (m_newFolderName->IsLostFocus)
			{
				if (!m_newFolderName->Text.empty())
				{
					fs::path newPath = currentDir / m_newFolderName->Text;
					if (!fs::exists(newPath))
						fs::create_directory(newPath);
				}
				m_newFolderName = std::nullopt;
			}
		}
	}
}

REGISTER_EDITOR_PANEL(AssetsPanel)
