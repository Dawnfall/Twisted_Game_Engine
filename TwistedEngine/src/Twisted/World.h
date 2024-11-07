#pragma once

#include "twistedpch.h"
#include "Game/EcsManager.h"
#include "Managers/TimeManager.h"
#include "Twisted/Game/Systems/RenderSystem.h"

#include "Serialization/Serializer.h"

namespace Twisted
{
	class CTransform;
	class AppBase;

	class World
	{
	public:
		World()
		{
			m_time.Init();
		}

		void UpdateFrame(AppBase* app);

		EcsManager& GetEcs() { return m_ecs; }
		TimeManager& GetTime() { return m_time; }
		std::vector<CTransform*> GetRootTransforms();
		CTransform& CreateNewEntity();

		template<typename T>
		void AddSystem()
		{
			static_assert(std::is_base_of <SystemBase, T>::value, "T must derive from SystemBase");
			m_systems.emplace_back(std::make_unique<T>());
		}

		static std::shared_ptr<World> CreateNew()
		{
			std::shared_ptr<World> newWorld = std::make_shared<World>();
			newWorld->AddSystem<Twisted::RenderSystem>(); //TODO: somewhere else
			return newWorld;
		}

	private:
		EcsManager m_ecs;
		std::vector<std::unique_ptr<SystemBase>> m_systems;
		TimeManager m_time;

	};

	//template<>
	//Buffer Serialize<World>(const World& world)
	//{
	//	//systems , ecs
	//}

	//template<>
	//World Deserialize<World>(const Buffer& buffer)
	//{

	//}
}
