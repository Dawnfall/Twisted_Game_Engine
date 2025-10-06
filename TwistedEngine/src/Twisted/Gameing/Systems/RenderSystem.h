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

	private:
		RenderLayer* m_renderLayer;
	};
}