#pragma once

#include "AppCore.h"
#include "Serialization/BinSerializer.h"

#include "Twisted/Gameing/Entity.h"
#include "yaml-cpp/yaml.h"
#include "Debug/Logger.h"
#include <stdexcept>
namespace Twisted
{
	template<typename T>
	void OnCreateComponent(T& component)= delete;
	template<typename T>
	void OnDestroyComponent(T& component) = delete;

	class World;
	class TWISTED_API AComponent
	{
	public:
		AComponent(Entity entity) :
			entity(entity)
		{
		}

		const World* GetWorld()const { return entity.GetWorld(); }
		World* GetWorld() { return entity.GetWorld(); }
		const EntityID& GetID()const { return entity.GetID(); }
		Entity entity;
	};
}




