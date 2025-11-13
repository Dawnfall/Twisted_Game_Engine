#pragma once
#include "AppCore.h"
#include "Twisted/Gameing/AComponent.h"
#include "Twisted/Gameing/WorldRegistry.h"

namespace Twisted
{
	class TWISTED_API CMainCamera :public AComponent
	{
	public:
		CMainCamera(Entity entity):AComponent(entity){}

		YAML::Node YamlSerialize() const override{ return YAML::Node(); }
		void YamlDeserialize(const YAML::Node& node) override{};
	};
}

REGISTER_COMPONENT(CMainCamera, "CMainCamera");

