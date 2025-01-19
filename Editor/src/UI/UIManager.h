#pragma once

#include <AppCore.h>
#include <vector>

#include "UI/FileBrowser/FileBrowser.h"
#include "UI/Panels/EditorPanel.h"

#include "UI/Panels/TreeViewPanel.h"
#include "UI/Panels/WorldViewPanel.h"
#include "UI/Panels/DetailsPanel.h"

namespace Twisted::Editor
{
	class EditorApp;
	class UIManager
	{
	public:
		UIManager(EditorApp* editor)
		{
			Editor = editor;

			CreateEditorPanel<TreeViewPanel>();
			CreateEditorPanel<DetailsPanel>();
			CreateEditorPanel<WorldViewPanel>();
		}

		template<typename T>
		void CreateEditorPanel()
		{
			static_assert(std::is_base_of<EditorPanel, T>::value, "T must be derived from EditorPanel");
			Panels.emplace_back(std::make_unique<T>(Editor));
		}

		template<typename T>
		T* GetPanel()
		{
			static_assert(std::is_base_of<EditorPanel, T>::value, "T must be derived from EditorPanel");
			for (auto& panel : Panels)
				if (T* castedPanel = dynamic_cast<T*>(panel.get()))
					return castedPanel;
			return nullptr;
		}

		void RenderUI();

		void RenderDockSpace();
		void RenderMenuBar();
		void RenderModals();

		EditorApp* Editor;
		std::vector<URef<EditorPanel>> Panels;
		URef<FileBrowser> m_fileBrowser = nullptr;
	};
}