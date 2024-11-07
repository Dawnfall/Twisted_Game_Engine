#pragma once
#include "editorpch.h"
#include "Twisted/AppBase.h"
#include "UI/Panels/EditorPanel.h"


namespace Twisted::Editor
{
	class EditorApp :public AppBase
	{
	public:

		void SelectEntity(entt::entity newEntity)
		{
			if (m_selectedEntityID == newEntity)
				return;
			m_selectedEntityID = newEntity;
		}
		entt::entity GetSelectedEntity()const { return m_selectedEntityID; }

		template<typename T>
		void CreateEditorPanel()
		{
			static_assert(std::is_base_of<EditorPanel, T>::value, "T must be derived from EditorPanel");
			auto newPanel = std::make_unique<T>();
			m_panels.emplace_back(std::move(newPanel));
		}

	protected:
		void CreateNewWindow(const AppParams& params) override;
		void RenderUI();
		void Run() override;

		void RenderDockSpace();
		void RenderMenuBar();
	protected:
		std::vector<std::unique_ptr<EditorPanel>> m_panels;
		entt::entity m_selectedEntityID = entt::null;

	};

}