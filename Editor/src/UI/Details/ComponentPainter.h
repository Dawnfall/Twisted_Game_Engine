#pragma once

namespace Twisted
{
	class World;
}

namespace Twisted::Editor
{
	class ComponentPainter
	{
	public:
		virtual void AddComponent(EntityID entity, World* world) = 0;
		virtual void* GetComponent(EntityID entity,World* world)const = 0;
		virtual void Paint(void* obj) = 0;
		virtual std::string GetComponentName()const = 0;
	};
}