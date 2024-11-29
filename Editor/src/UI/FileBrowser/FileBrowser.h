#pragma once
#include "editorpch.h"
#include "UI/FileBrowser/IconDatabase.h"

#include "Twisted/Rendering/Texture.h"
#include "Assets/TextureData.h"
#include "Twisted/Rendering/RenderingAPI.h"

namespace Twisted::Editor
{
	enum class BrowserType
	{
		FILE,
		DIRECTORY,
		SAVE
	};
	class FileFilter
	{
	public:
		FileFilter(const std::string& name, const std::vector<std::string>& allowedExtensions) :
			m_allowedExtensions(allowedExtensions),
			m_filterName(name)
		{
		}

		bool VerifyExt(const std::filesystem::path& path)
		{
			if (std::filesystem::exists(path) && path.has_extension())
			{
				if (m_allowedExtensions.size() == 0)
					return true;

				auto it = std::find(m_allowedExtensions.begin(), m_allowedExtensions.end(), path.extension());
				if (it != m_allowedExtensions.end())
					return true;
			}
			return false;
		}

		std::string& Name() { return m_filterName; }
	private:
		std::vector<std::string> m_allowedExtensions;
		std::string m_filterName = "";
	};
	struct FileBrowserData
	{
		using OnConfirm = std::function<bool(const std::vector<fs::path>&)>;

		std::string Title = "";
		BrowserType BrowserType;
		fs::path StartDir;
		std::vector<FileFilter> Filters = std::vector<FileFilter>();
		bool IsMultiselect;

		OnConfirm onConfirm = [](const std::vector<fs::path>&) {return true; };
	};


	class FileBrowser
	{
	public:
		FileBrowser(const FileBrowserData data);
		bool Render();

		std::vector<fs::path>& GetResults() { return m_results; }

	private:
		//Render
		void RenderBrowser();
		void RenderPopups();
		void RenderTree(fs::path& dir);
		void RenderContent();

		bool OnConfirm(const fs::path& path);
		void OnSelect(const fs::path& path, bool doMultiSelect);
		void OnDoubleClickSelect(const fs::path& path);
		void OnSingleClickSelect(const fs::path& path);
		void OnMouseRoll();

		void SetDirectory(const fs::path& path, bool saveHistory);
		void RefreshDirectory();
		void SortContent(int column, int sortDirection);
		void Close();

	private:
		FileBrowserData Data;
		std::vector<fs::path> m_results;

		FileFilter& GetActiveFilter() { return Data.Filters[m_activeFilterIndex]; }
		std::shared_ptr<Texture> GetTexture(const fs::path& path, bool prefferPreview);
		bool IsSelected(const fs::path& path) { return m_selectedContent.contains(path); }

		//Favorites
		void AddFavorite(const fs::path& path);
		void RemoveFavorite(const fs::path& path);
		bool IsFavourite(const fs::path& path);
		std::vector<fs::path> m_favorites;

		//Navigation
		std::stack<fs::path> m_backHistory, m_forwardHistory;
		void GoBackHistory();
		void GoForwardHistory();
		void GoUpDir();

		// .... Internals

		bool m_isOpened = true;
		bool m_justOpened = true;
		// Zoom
		float m_zoom;

		// Inputs

		char m_searchBuffer[1024] = "";
		char m_inputTextBuffer[256] = "";
		char m_pathBuffer[1024] = "";
		char m_newEntryBuffer[1024] = "";

		IconDatabase m_iconDatabase;

		//Tree
		std::vector<fs::path> m_drives;

		//Content
		fs::path m_currentDir;
		std::vector<fs::path> m_currentContent;

		//Selections
		std::unordered_set<fs::path> m_selectedContent;

		//Filters
		int m_activeFilterIndex = 0;
	};
}