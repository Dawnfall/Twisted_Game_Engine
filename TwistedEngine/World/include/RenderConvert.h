#pragma once
#include "AppCore.h"
#include "Data/RenderContext.h"

#include <vector>

namespace Twisted
{
	class World;

	TWISTED_API std::vector<CameraData> CollectCameraData(World& world);
	TWISTED_API std::vector<ModelData>  CollectModelData(World& world);
	TWISTED_API LightData               CollectLightData(World& world);
	TWISTED_API RenderContext           ExtractContext(World& world);
}
