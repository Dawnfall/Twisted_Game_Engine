#pragma once
#include "Twisted/Application/Layer.h"
#include "LoadupConfig.h"
#include "EditorConfig.h"
#include "Twisted/Gameing/World.h"
#include "UI/PanelManager.h"

#include <vector>
#include <unordered_set>
#include <variant>

#include "Twisted/Rendering/FrameBuffer.h"
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

	private:

		void RenderDockSpace();
		void RenderMenuBar(Window* window);
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