#pragma once
//
//#include "AppCore.h"
//
#include <unordered_set>
#include <filesystem>
#include <variant>
#include "Twisted/Gameing/Entity.h"
#include "Twisted/TObject.h"

//// Helper trait to detect std::unordered_set
////template<typename>
////struct is_unordered_set : std::false_type {};
////
////template<typename... Args>
////struct is_unordered_set<std::unordered_set<Args...>> : std::true_type {};
//

enum SelectionFlags :uint32_t
{
	NONE = 0,
	REMOVE_OTHERS = 1 << 0,
	REMOVE_IF_SELECTED = 2 << 0
};

enum class SelectionType
{
	NONE,
	ENTITY,
	ASSET,
	OBJECT
};


namespace fs = std::filesystem;
namespace Twisted::Editor
{
	class Selection
	{
	public:
		SelectionType GetActiveType()const { return m_activeType; }

		const std::unordered_set<ObjectID>& GetSelectedObjects()const { return m_selectedObjects; }
		void SelectObject(const std::vector<TObject*>& selected, SelectionFlags flags = SelectionFlags::NONE)
		{
			if (flags & SelectionFlags::REMOVE_OTHERS)
			{
				m_selectedObjects.clear();
			}

			for (auto& obj : selected)
			{
				if (flags & SelectionFlags::REMOVE_IF_SELECTED)
				{
					if (!m_selectedObjects.erase(obj->GetID()))
						m_selectedObjects.emplace(obj->GetID());
				}
				else
				{
					m_selectedObjects.emplace(obj->GetID());
				}
			}
			m_activeType = SelectionType::OBJECT;
		}
		void DeselectObjects(const std::vector<TObject*>& selected)
		{
			for (auto& obj : selected)
				m_selectedObjects.erase(obj->GetID());
		}
		void ClearObjects()
		{
			m_selectedObjects.clear();
		}

		const std::unordered_set<fs::path>& GetSelectedPaths()const { return m_selectedPaths; }
		void SelectPaths(const std::vector<std::filesystem::path>& selected, SelectionFlags flag = SelectionFlags::NONE)
		{
			if (flag & SelectionFlags::REMOVE_OTHERS)
			{
				m_selectedPaths.clear();
			}

			for (auto& path : selected)
			{
				if (flag & SelectionFlags::REMOVE_IF_SELECTED)
				{
					if (!m_selectedPaths.erase(path))
						m_selectedPaths.emplace(path);
				}
				else
				{
					m_selectedPaths.emplace(path);
				}
			}
			m_activeType = SelectionType::ASSET;
		}

		void DeselectPaths(const std::vector<fs::path>& selected)
		{
			for (auto& path : selected)
				m_selectedPaths.erase(path);
		}

		void ClearPaths()
		{
			m_selectedPaths.clear();
		}

		const std::unordered_set<Entity>& GetSelectedEntities() const { return m_selectedEntities; }
		void SelectEntities(const std::vector<Entity>& selected, SelectionFlags flag = SelectionFlags::NONE)
		{
			if (flag & SelectionFlags::REMOVE_OTHERS)
			{
				m_selectedEntities.clear();
			}

			for (auto& entity : selected)
			{
				if (flag & SelectionFlags::REMOVE_IF_SELECTED)
				{
					if (!m_selectedEntities.erase(entity))
						m_selectedEntities.emplace(entity);
				}
				else
				{
					m_selectedEntities.emplace(entity);
				}
			}
			m_activeType = SelectionType::ENTITY;
		}
		void DeselectEntites(const std::vector<Entity>& selected)
		{
			for (auto& entity : selected)
				m_selectedEntities.erase(entity);
		}
		void ClearEntities()
		{
			m_selectedEntities.clear();
		}

		void CleanAllSelected()
		{
			ClearEntities();
			m_selectedPaths.clear();
		}

	private:
		SelectionType m_activeType=SelectionType::NONE;

		std::unordered_set<ObjectID> m_selectedObjects;
		std::unordered_set<Entity> m_selectedEntities;
		std::unordered_set<std::filesystem::path> m_selectedPaths;
	};
}

