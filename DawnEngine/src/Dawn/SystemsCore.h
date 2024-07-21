#pragma once

#include "Dawn/Game/SystemBase.h"
#include <vector>
#include <memory>

namespace Dawn
{
	class SystemsCore
	{
	public:
		void UpdateSystems()
		{
			for (auto& system : Systems)
				system->Update();
		}

		template<typename T>
		void AddSystem()
		{
			static_assert(std::is_base_of <SystemBase, T>::value, "T must derive from SystemBase");
			Systems.push_back(std::make_unique<T>);
		}

	private:
		std::vector<std::unique_ptr<SystemBase>> Systems;

	};
}