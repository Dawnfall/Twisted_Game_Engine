#pragma once
#include "Twisted/Application/Layer.h"
#include "LoadupConfig.h"
#include "EditorConfig.h"
#include "Twisted/Gameing/World.h"
#include "Selection.h"
#include "UI/PanelManager.h"

#include <vector>
#include <unordered_set>
#include <variant>

namespace Twisted
{
	class GameLayer;
	class ObjectManager;
	class World;
	class Window;
	class Application;
}

namespace Twisted::Editor
{
	inline const std::string mainDockSpaceLabel = "MainDockSpace";
	inline ImGuiWindowFlags panelFlags =
		ImGuiWindowFlags_NoCollapse;     // optional: prevent collapsing
		//| ImGuiWindowFlags_NoDocking;   // prevents undocking

	inline ImGuiWindowFlags dockFlags =
		  ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoNavFocus;

	class EditorLayer :public Layer
	{
	public:

		EditorLayer(Application* app) :
			Layer(app)
		{
		}

		void Init() override;
		void Render(Window* window);

		void SetWorld(World* world);
		World* GetGameWorld() { return ObjectManager::GetInstance().GetIdObject<World>(m_gameWorld); }
		World* GetEditorWorld() { return ObjectManager::GetInstance().GetIdObject<World>(m_editorWorld); }
		Event<> WorldChangeEvent;

		//*********
		// CONFIGS
		//********

		EditorConfig EditorConfigData;

		Selection& GetSelection() { return m_selection; }

		template<typename T>
		void CreateEditorPanel()
		{
			static_assert(std::is_base_of<EditorPanel, T>::value, "T must be derived from EditorPanel");
			m_panels.emplace_back(std::make_unique<T>(this));
		}

	private:

		void RenderDockSpace();
		void RenderMenuBar(Window* window);

		Selection m_selection;

		std::vector<URef<EditorPanel>> m_panels;

	private:

		//*********
		// Active WORLD
		//*********

		ObjectID m_gameWorld;
		ObjectID m_editorWorld;
	};
}

//void MultiEntitySelect(const std::vector<EntityID>& entities)
	//{
	//	m_selectedEntities.insert(entities.begin(), entities.end());
	//}
	//void SingleEntitySelect(EntityID ent)
	//{
	//	if (m_selectedEntities.size() == 1 && m_selectedEntities.contains(ent))
	//		return;
	//	m_selectedEntities.clear();
	//	m_selectedEntities.emplace(ent);
	//}
	//void DeselectEntity(EntityID ent)
	//{
	//	m_selectedEntities.erase(ent);
	//}
	//void ClearSelectedEntities() { m_selectedEntities.clear(); }