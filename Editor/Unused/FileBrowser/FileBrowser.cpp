#include "editorpch.h"
#include "FileBrowser.h"
#include "UI/ImguiExtensions.h"
#include "Utils/FileUtils.h"
#include "Utils/Utils.h"

namespace rend = Twisted::Render;

namespace Twisted::Editor
{
	FileBrowser::FileBrowser(const FileBrowserData data) : //TODO: reference
		Data(data),
		m_zoom(1)
	{
		m_drives = Utils::GetAllDrives();

		if (Data.Filters.empty())
			Data.Filters.emplace_back("default", std::vector<std::string>());

		SetDirectory(Data.StartDir, false);
	}


	bool FileBrowser::Render()
	{
		if (m_isOpened)
		{
			if (m_justOpened)
			{
				//TODO... remove
				ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
				ImGui::OpenPopup(Data.Title.c_str());
				m_justOpened = false;
			}

			if (ImGui::BeginPopupModal(Data.Title.c_str(), &m_isOpened, ImGuiWindowFlags_NoScrollbar))
			{
				RenderBrowser();
				ImGui::EndPopup();
			}
			else
				Close();
		}
		return m_isOpened;
	}

	void FileBrowser::SetDirectory(const fs::path& path, bool saveHistory)
	{
		if (m_currentDir == path)
			return;
		if (saveHistory)
			m_backHistory.push(m_currentDir);

		m_currentDir = path;
		RefreshDirectory();
	}

	void FileBrowser::RefreshDirectory()
	{
		m_currentContent = Utils::SubEntries(m_currentDir);
		m_iconDatabase.Clear();
		m_selectedContent.clear();
	}

	void FileBrowser::Close()
	{
		m_isOpened = false;
	}

	std::shared_ptr<rend::Texture> FileBrowser::GetTexture(const fs::path& path, bool prefferPreview)
	{
		if (prefferPreview)
		{
			auto previewTex = m_iconDatabase.GetPreviewTexture(path);
			if (previewTex)
				return previewTex;
		}
		return m_iconDatabase.GetIconTexture(path);
	}

	void FileBrowser::OnSelect(const fs::path& path, bool doMultiSelect)
	{
		if (m_selectedContent.contains(path))
		{
			m_selectedContent.erase(path);
		}
		else if (m_selectedContent.size() == 0)
		{
			m_selectedContent.insert(path);
			return;
		}
		else if (doMultiSelect && Data.IsMultiselect)
		{
			m_selectedContent.insert(path);
		}
		else
		{
			m_selectedContent.clear();
			m_selectedContent.insert(path);
		}

		if (m_selectedContent.size() == 1)
			Utils::StringToArray(path.filename().string(), m_inputTextBuffer, sizeof(m_inputTextBuffer));
	}

	bool FileBrowser::OnConfirm(const fs::path& path)
	{
		if (Data.BrowserType == BrowserType::DIRECTORY)
		{
			m_results.emplace_back(m_currentDir);
			if (Data.onConfirm(m_results))
			{
				Close();
				return true;
			}
		}
		if (Data.BrowserType == BrowserType::FILE)
		{
			for (auto& p : m_selectedContent)
				m_results.emplace_back(p);
			if (Data.onConfirm(m_results))
			{
				Close();
				return true;
			}
		}
		if (Data.BrowserType == BrowserType::SAVE)
		{
			fs::path savePath = m_currentDir / m_inputTextBuffer;
			if (savePath.has_filename() && !savePath.filename().empty())
				if (!std::filesystem::exists(savePath))
				{
					m_results.emplace_back(savePath);
					if (Data.onConfirm(m_results))
					{
						Close();
						return true;
					}
				}
		}
		m_results.clear();
		return false;
	}
	void FileBrowser::OnSingleClickSelect(const fs::path& path)
	{
		if ((fs::is_directory(path) && Data.BrowserType == BrowserType::DIRECTORY) || !fs::is_directory(path))
			OnSelect(path, ImGui::GetIO().KeyCtrl);
	}
	void FileBrowser::OnDoubleClickSelect(const fs::path& path)
	{
		if (fs::is_directory(path))
			SetDirectory(path, true);
		else
			OnConfirm(path);
	}
	void FileBrowser::OnMouseRoll()
	{
		m_zoom = std::min<float>(25.0f, std::max<float>(1.0f, m_zoom + ImGui::GetIO().MouseWheel));

	}

	void FileBrowser::SortContent(int column, int sortDirection) // 0 column , 1 date , 2 size
	{
		//unsigned int column = m_sortColumn; //TODO: not needed
		//unsigned int sortDirection = m_sortDirection;
		if (m_currentContent.size() > 0)
		{
			// compare function
			auto compareFn = [column, sortDirection](const fs::path& left, const fs::path& right) -> bool
				{
					if (Utils::IsFolder(left) && Utils::IsFile(right))
						return true;
					if (Utils::IsFolder(right) && Utils::IsFile(left))
						return false;

					// name
					if (column == 0) {
						std::string lName = left.string();
						std::string rName = right.string();

						std::transform(lName.begin(), lName.end(), lName.begin(), ::tolower);
						std::transform(rName.begin(), rName.end(), rName.begin(), ::tolower);

						int comp = lName.compare(rName);

						if (sortDirection == ImGuiSortDirection_Ascending)
							return comp < 0;
						return comp > 0;
					}
					// date
					else if (column == 1) {
						if (sortDirection == ImGuiSortDirection_Ascending)
							return Utils::DateModified(left) < Utils::DateModified(right);
						else
							return Utils::DateModified(left) > Utils::DateModified(right);
					}
					// size
					else if (column == 2) {
						if (sortDirection == ImGuiSortDirection_Ascending)
							return Utils::FileSize(left) < Utils::FileSize(right);
						else
							return Utils::FileSize(left) > Utils::FileSize(right);
					}

					return false;
				};
			std::sort(m_currentContent.begin(), m_currentContent.end(), compareFn);
		}
	}
	void FileBrowser::GoBackHistory()
	{
		std::filesystem::path newPath = m_backHistory.top();
		m_backHistory.pop();
		m_forwardHistory.push(m_currentDir);

		SetDirectory(newPath, false);
	}
	void FileBrowser::GoForwardHistory()
	{
		std::filesystem::path newPath = m_forwardHistory.top();
		m_forwardHistory.pop();
		m_backHistory.push(m_currentDir);

		SetDirectory(newPath, false);
	}
	void FileBrowser::GoUpDir()
	{
		if (m_currentDir.has_parent_path())
			SetDirectory(m_currentDir.parent_path(), true);
	}

	void FileBrowser::AddFavorite(const fs::path& path)
	{
		auto itr = std::find(m_favorites.begin(), m_favorites.end(), path);
		if (itr == m_favorites.end())
			m_favorites.emplace_back(path);
	}
	void FileBrowser::RemoveFavorite(const fs::path& path)
	{
		auto itr = std::find(m_favorites.begin(), m_favorites.end(), path);
		if (itr != m_favorites.end())
			m_favorites.erase(itr);
	}
	bool FileBrowser::IsFavourite(const fs::path& path)
	{
		auto itr = std::find(m_favorites.begin(), m_favorites.end(), path);
		return itr != m_favorites.end();
	}


	void FileBrowser::RenderBrowser()
	{
		/***** TOP BAR *****/
		bool noBackHistory = m_backHistory.empty(), noForwardHistory = m_forwardHistory.empty();

		ImGui::PushStyleColor(ImGuiCol_Button, 0);
		if (noBackHistory)
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		if (ImGui::ArrowButtonEx("##back", ImGuiDir_Left, ImVec2(Im::GUI_ELEMENT_SIZE(), Im::GUI_ELEMENT_SIZE()), m_backHistory.empty() * ImGuiItemFlags_Disabled))
			GoBackHistory();
		if (noBackHistory)
			ImGui::PopStyleVar();
		ImGui::SameLine();

		if (noForwardHistory)
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		if (ImGui::ArrowButtonEx("##forward", ImGuiDir_Right, ImVec2(Im::GUI_ELEMENT_SIZE(), Im::GUI_ELEMENT_SIZE()), m_forwardHistory.empty() * ImGuiItemFlags_Disabled))
			GoForwardHistory();
		if (noForwardHistory) ImGui::PopStyleVar();
		ImGui::SameLine();

		if (ImGui::ArrowButtonEx("##up", ImGuiDir_Up, ImVec2(Im::GUI_ELEMENT_SIZE(), Im::GUI_ELEMENT_SIZE())))
			GoUpDir();

		std::filesystem::path curDirCopy = m_currentDir;
		if (Im::PathBox("##pathbox", curDirCopy, &m_pathBuffer[0], ImVec2(-250, Im::GUI_ELEMENT_SIZE())))
			SetDirectory(curDirCopy, true);
		ImGui::SameLine();

		bool isFavourite = IsFavourite(m_currentDir);
		if (Im::FavoriteButton("##dirfav", isFavourite)) {
			if (isFavourite)
				RemoveFavorite(m_currentDir);
			else
				AddFavorite(m_currentDir);
		}
		ImGui::SameLine();
		ImGui::PopStyleColor();

		if (ImGui::InputTextEx("##searchTB", "Search", m_searchBuffer, sizeof(m_searchBuffer), ImVec2(-FLT_MIN, Im::GUI_ELEMENT_SIZE()), 0)) // TODO: no hardcoded literals
			SetDirectory(m_currentDir, false); // refresh 

		/***** CONTENT *****/
		float bottomBarHeight = (GImGui->FontSize + ImGui::GetStyle().FramePadding.y + ImGui::GetStyle().ItemSpacing.y * 2.0f) * 2;
		if (ImGui::BeginTable("##table", 2, ImGuiTableFlags_Resizable, ImVec2(0, -bottomBarHeight)))
		{
			ImGui::TableSetupColumn("##tree", ImGuiTableColumnFlags_WidthFixed, 125.0f);
			ImGui::TableSetupColumn("##content", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableNextRow();

			// the tree on the left side
			ImGui::TableSetColumnIndex(0);
			ImGui::BeginChild("##treeContainer", ImVec2(0, -bottomBarHeight));

			for (fs::path& dir : m_drives)
				RenderTree(dir);
			ImGui::EndChild();

			// content on the right side
			ImGui::TableSetColumnIndex(1);
			ImGui::BeginChild("##contentContainer", ImVec2(0, -bottomBarHeight));
			RenderContent();
			ImGui::EndChild();

			if (ImGui::IsItemHovered() && ImGui::GetIO().KeyCtrl && ImGui::GetIO().MouseWheel != 0.0f)
			{
				OnMouseRoll();
				//TODO... m_refreshIconPreview();
			}

			// New file, New directory and Delete popups
			RenderPopups();

			ImGui::EndTable();
		}

		/***** BOTTOM BAR *****/
		ImGui::Text("File name:");
		ImGui::SameLine();
		if (ImGui::InputTextEx("##file_input", "Filename", m_inputTextBuffer, sizeof(m_inputTextBuffer), ImVec2((Data.BrowserType != BrowserType::DIRECTORY) ? -250.0f : -FLT_MIN, 0), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			bool success = OnConfirm(m_inputTextBuffer);

			if (!success)
				MessageBeep(MB_ICONERROR);
		}
		if (Data.BrowserType != BrowserType::DIRECTORY)
		{
			ImGui::SameLine();
			ImGui::SetNextItemWidth(-FLT_MIN);
			if (ImGui::Combo("##ext_combo", &m_activeFilterIndex, GetActiveFilter().Name().c_str()))
			{
				SetDirectory(m_currentDir, false); // refresh
			}
		}


		// buttons
		float ok_cancel_width = Im::GUI_ELEMENT_SIZE() * 7;
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ok_cancel_width);
		if (ImGui::Button(Data.BrowserType == BrowserType::SAVE ? "Save" : "Open", ImVec2(ok_cancel_width / 2 - ImGui::GetStyle().ItemSpacing.x, 0.0f)))
		{
			bool success = OnConfirm(fs::path(m_inputTextBuffer));

			if (!success)
				MessageBeep(MB_ICONERROR);

		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(-FLT_MIN, 0.0f)))
		{
			Close();
		}

		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImGuiKey_Escape >= 0 && ImGui::IsKeyPressed(ImGuiKey_Escape))
			Close();

	}
	void FileBrowser::RenderContent()
	{
		//if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		//	m_selectedContentIndex = -1;

		int clickCount = 0;
		fs::path selectedPath;
		// table view
		if (m_zoom == 1.0f)
		{
			if (ImGui::BeginTable("##contentTable", 3, /*ImGuiTableFlags_Resizable |*/ ImGuiTableFlags_Sortable, ImVec2(0, -FLT_MIN)))
			{
				// header
				ImGui::TableSetupColumn("Name##filename", ImGuiTableColumnFlags_WidthStretch, 0.0f - 1.0f, 0);
				ImGui::TableSetupColumn("Date modified##filedate", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 0.0f, 1);
				ImGui::TableSetupColumn("Size##filesize", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 0.0f, 2);
				ImGui::TableSetupScrollFreeze(0, 1);
				ImGui::TableHeadersRow();

				// sort
				if (ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs())
				{
					if (sortSpecs->SpecsDirty)
					{
						SortContent(sortSpecs->Specs->ColumnIndex, sortSpecs->Specs->SortDirection);
						sortSpecs->SpecsDirty = false;
					}
				}

				// content
				for (fs::path path : m_currentContent)
				{
					std::string filename = path.filename().string();
					if (filename.size() == 0)
						filename = path.string(); // drive

					ImGui::TableNextRow();

					// file name
					ImGui::TableSetColumnIndex(0);
					ImGui::Image((ImTextureID)m_iconDatabase.GetIconTexture(path)->TextureID, ImVec2(Im::ICON_SIZE(), Im::ICON_SIZE()));
					ImGui::SameLine();
					if (ImGui::Selectable(filename.c_str(), IsSelected(path), ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick))
					{
						if (ImGui::IsMouseDoubleClicked(0))
							clickCount = 2;
						else
							clickCount = 1;
						selectedPath = path;

					}
					//if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
					//	m_selectedContentIndex = fileId;

					// date
					ImGui::TableSetColumnIndex(1);
					time_t dateModified = Utils::DateModified(path);
					auto tm = std::localtime(&dateModified);
					if (tm != nullptr)
						ImGui::Text("%d/%d/%d %02d:%02d", tm->tm_mon + 1, tm->tm_mday, 1900 + tm->tm_year, tm->tm_hour, tm->tm_min);
					else ImGui::Text("---");

					// size
					ImGui::TableSetColumnIndex(2);
					ImGui::Text("%.3f KiB", Utils::FileSize(path) / 1024.0f);
				}

				ImGui::EndTable();
			}
		}
		// "icon" view
		else
		{
			// content
			for (fs::path& path : m_currentContent)
			{
				std::string filename = path.filename().string();
				if (filename.size() == 0)
					filename = path.string(); // drive


				auto tex = GetTexture(path, true);
				ImVec2 size(32 + 16 * m_zoom, 32 + 16 * m_zoom);
				if (Im::FileIcon(filename.c_str(), IsSelected(path), (ImTextureID)tex->TextureID, size))
				{
					if (ImGui::IsMouseDoubleClicked(0))
						clickCount = 2;
					else
						clickCount = 1;
					selectedPath = path;
				}
				//if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				//	m_selectedContentIndex = fileId;
			}
		}
		if (clickCount == 1)
			OnSingleClickSelect(selectedPath);
		else if (clickCount == 2)
			OnDoubleClickSelect(selectedPath);
	}
	void FileBrowser::RenderTree(fs::path& path)
	{
		// directory
		std::error_code ec;
		ImGui::PushID(&path);
		bool isClicked = false;
		std::string displayName = path.stem().string();
		if (displayName.size() == 0)
			displayName = path.string();
		if (Im::FolderNode(displayName.c_str(), (ImTextureID)m_iconDatabase.GetIconTexture(path)->TextureID, isClicked))
		{
			// display children
			for (fs::path& childPath : Utils::SubFolders(path))
				RenderTree(childPath);

			ImGui::TreePop();
		}
		if (isClicked)
			SetDirectory(path, true);
		ImGui::PopID();
	}

	void FileBrowser::RenderPopups()
	{
		bool openAreYouSureDlg = false, openNewFileDlg = false, openNewDirectoryDlg = false;
		if (ImGui::BeginPopupContextItem("##dir_context")) {
			if (ImGui::Selectable("New file"))
				openNewFileDlg = true;
			if (ImGui::Selectable("New directory"))
				openNewDirectoryDlg = true;
			if (m_selectedContent.size() > 0 && ImGui::Selectable("Delete"))
				openAreYouSureDlg = true;
			ImGui::EndPopup();
		}
		if (openAreYouSureDlg)
			ImGui::OpenPopup("Are you sure?##delete");
		if (openNewFileDlg)
			ImGui::OpenPopup("Enter file name##newfile");
		if (openNewDirectoryDlg)
			ImGui::OpenPopup("Enter directory name##newdir");
		if (ImGui::BeginPopupModal("Are you sure?##delete"))
		{
			if (m_selectedContent.size() == 0)
				ImGui::CloseCurrentPopup();
			else
			{
				ImGui::TextWrapped("Are you sure you want to delete ?"); //TODO: maybe different output for 1 vs many files
				if (ImGui::Button("Yes"))
				{
					std::error_code ec;
					for (auto& path : m_selectedContent)
						std::filesystem::remove_all(path, ec);

					RefreshDirectory(); // refresh
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("No"))
					ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopupModal("Enter file name##newfile")) {
			ImGui::PushItemWidth(250.0f);
			ImGui::InputText("##newfilename", m_newEntryBuffer, sizeof(m_newEntryBuffer)); // TODO: remove hardcoded literals
			ImGui::PopItemWidth();

			if (ImGui::Button("OK")) {
				std::ofstream out(m_currentDir / m_newEntryBuffer);
				out << "";
				out.close();

				SetDirectory(m_currentDir, false); // refresh
				m_newEntryBuffer[0] = 0;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				m_newEntryBuffer[0] = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopupModal("Enter directory name##newdir")) {
			ImGui::PushItemWidth(250.0f);
			ImGui::InputText("##newfilename", m_newEntryBuffer, sizeof(m_newEntryBuffer));
			ImGui::PopItemWidth();

			if (ImGui::Button("OK"))
			{
				std::error_code ec;
				std::filesystem::create_directory(m_currentDir / m_newEntryBuffer, ec);
				SetDirectory(m_currentDir, false); // refresh
				m_newEntryBuffer[0] = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
				m_newEntryBuffer[0] = 0;
			}
			ImGui::EndPopup();
		}
	}


}