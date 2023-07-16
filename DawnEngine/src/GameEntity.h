#pragma once

#include "Dawn/Components/Transform.h"
#include "Dawn/AComponent.h"

#include <vector>

namespace Dawn
{
	class AComponent;

	class GameEntity
	{
	public:
		Transform _transform;
		std::vector<AComponent*> m_components;

		bool RemoveComponent(const AComponent& component)
		{
			for (AComponent* comp : m_components)
			{
				
			}
			//auto it = std::find(m_components.begin(), m_components.end(), component);
			//if (it != m_components.end()) {
			//	m_components.erase(it);
			//	return true;
			//}
			return false;
		}

		template <typename T>
		void AddComponent()
		{
			static_assert(std::is_base_of<AComponent, T>::value);
			m_components.emplace_back(T(this));
		}



		//template <typename T>
		//T* GetComponent()
		//{
		//	static_assert(std::is_base_of<AComponent, T>::value);
		//	for (const AComponent& comp : m_components)
		//	{
		//		if (T* b = dynamic_cast<T*>(&comp))
		//		{
		//			return b;
		//		}
		//	}
		//	return nullptr;
		//}


	private:
	};
}
