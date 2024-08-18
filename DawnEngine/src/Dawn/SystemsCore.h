#pragma once

#include "Dawn/Game/SystemBase.h"
#include <vector>
#include <memory>

namespace Dawn
{
	class Application;

	class SystemsCore
	{
	public:

		void UpdateSystems(Application& app)
		{
			for (auto& system : m_systems)
				system->Update(app);
		}

		template<typename T>
		void AddSystem()
		{
			static_assert(std::is_base_of <SystemBase, T>::value, "T must derive from SystemBase");
			m_systems.push_back(std::make_unique<T>());
			//m_systems.push_back(new T());
		}

	private:
		std::vector<std::unique_ptr<SystemBase>> m_systems;
		//std::vector<SystemBase*> m_systems;

	};
}