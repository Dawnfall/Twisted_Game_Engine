#pragma once
#include "AppCore.h"
#include "Twisted/Gameing/ManagerBase.h"
#include "Twisted/Gameing/Entity.h"
#include "Twisted/Gameing/WorldRegistry.h"
#include "Utils/YamlUtils.h"

namespace Twisted
{
	class CameraComponent;
	class TWISTED_API CameraManager:public ManagerBase
	{
	public:
		CameraManager(World* world) :ManagerBase(world) {}
		CameraComponent* GetMainCamera();
		void SetAsMainCamera(CameraComponent* cam);

	private:
		EntityID m_mainCamera = NullEntity;

		friend YAML::Node YamlSerialize<CameraManager>(const CameraManager&);
		friend void YamlDeserialize<CameraManager>(CameraManager&, const YAML::Node&);
	};

	template<>
	inline YAML::Node YamlSerialize<CameraManager>(const CameraManager& manager)
	{
		YAML::Node node;
		node["mainCamera"] = manager.m_mainCamera;
		return node;
	}

	template<>
	inline void YamlDeserialize<CameraManager>(CameraManager& manager, const YAML::Node& node)
	{
		if (node["mainCamera"])
			manager.m_mainCamera = node["mainCamera"].as<EntityID>(NullEntity);
	}
}

REGISTER_MANAGER(CameraManager, "CameraManager");