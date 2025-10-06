#pragma once

#include "AppCore.h"

#include <unordered_set>
#include <variant>
#include "Twisted/Gameing/Entity.h"

// Helper trait to detect std::unordered_set
template<typename>
struct is_unordered_set : std::false_type {};

template<typename... Args>
struct is_unordered_set<std::unordered_set<Args...>> : std::true_type {};

namespace Twisted::Editor
{
	using SelectionVar = std::variant<
		std::monostate,
		std::unordered_set<Entity>
	>;

	class Selection
	{
	public:
		
		static Selection& GetInstance()
		{
			static Selection selection;
			return selection;
		}

		template <typename T>
		void SingleSelect(const T& selection)
		{
			using SetType = std::unordered_set<T>;
			if (const auto* set = std::get_if<SetType>(&m_selection))
			{
				if (set->size() == 1 && set->contains(selection))
					return; // Already the only item selected
			}

			// Clear and select only this item
			SetType newSet;
			newSet.insert(selection);
			m_selection = std::move(newSet);
		}

		template<typename T>
		void MultiSelect(const std::vector<T> selections)
		{
			using SetType = std::unordered_set<T>;
			if (!std::holds_alternative<SetType>(m_selection))
			{
				// If current selection is not the same type, clear and start new set
				SetType newSet(selections.begin(), selections.end());
				m_selection = std::move(newSet);
				return;
			}

			// Otherwise, add only items not already present
			auto& set = std::get<SetType>(m_selection);
			for (const auto& item : selections)
				set.insert(item);
		}

		template<typename T>
		void DeSelect(T selection)
		{
			using SetType = std::unordered_set<T>;
			if (auto* set = std::get_if<SetType>(&m_selection))
			{
				set->erase(selection);
				if (set->empty())
					m_selection = std::monostate{};
			}
		}

		void Clear()
		{
			if (!Empty())
				m_selection = std::monostate{};
		}

		template<typename T>
		bool Contains(const T& item) const
		{
			using SetType = std::unordered_set<T>;
			if (const auto* set = std::get_if<SetType>(&m_selection))
				return set->contains(item);
			return false;
		}

		bool Empty()const
		{
			return std::holds_alternative<std::monostate>(m_selection);
		}

		SelectionVar& GetSelection() { return m_selection; }

	private:
		SelectionVar m_selection;
	};


}
