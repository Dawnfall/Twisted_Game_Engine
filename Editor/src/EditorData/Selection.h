#pragma once
//
//#include "AppCore.h"
//
#include <unordered_set>
#include <filesystem>
#include <variant>
#include "Twisted/Gameing/Entity.h"

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
	REMOVE_IF_SELECTED = 2 << 0,

};


namespace fs = std::filesystem;
namespace Twisted::Editor
{
	class Selection
	{
	public:
		const std::unordered_set<fs::path>& GetSelectedPaths() { return m_selectedPaths; }
		void SelectPaths(const std::vector<std::filesystem::path>& selected)
		{
			for (auto& path : selected)
				m_selectedPaths.emplace(path);
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

		const std::unordered_set<Entity>& GetSelectedEntities() { return m_selectedEntities; }

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
					if(!m_selectedEntities.erase(entity))
						m_selectedEntities.emplace(entity);
				}
				else
					m_selectedEntities.emplace(entity);
			}
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
		std::unordered_set<Entity> m_selectedEntities;
		std::unordered_set<std::filesystem::path> m_selectedPaths;
	};
}
