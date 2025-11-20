#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/SystemBase.h"
#include "Twisted/Rendering/RenderLayer.h"
#include <vector>
#include <unordered_map>
#include "Twisted/Gameing/World.h"

namespace Twisted
{
	class TWISTED_API RenderSystem :public SystemBase
	{
	public:
		RenderSystem(World* world);

		void Update() override;

		std::string GetTypeName()const override { return "RenderSystem"; }

	private:
		std::vector<CameraData> CollectCameraData(World* world)const;
		std::vector<ModelData> CollectModelData(World* world)const;
		LightData CollectLightData(World* world)const;

		RenderLayer* m_renderLayer;
	};
}

