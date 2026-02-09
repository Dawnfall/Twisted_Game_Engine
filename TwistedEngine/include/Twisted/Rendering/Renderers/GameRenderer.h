#pragma once
#include "AppCore.h"

#include "Twisted/Gameing/World.h"
#include "Twisted/Rendering/Data/ClearParams.h"
#include "Twisted/Rendering/Data/RenderContext.h"

#include <vector>


namespace Twisted
{
	class TWISTED_API GameRenderer
	{
	public:
		void Render(World& gameWorld)
		{
			RenderContext context = ExtractContext(gameWorld);
			//Render_GL::ForwardRender(context);
		}

		RenderContext ExtractContext(World& gameWorld)
		{
			RenderContext context{};

			context.camDatas = CollectCameraData(gameWorld);
			context.modelDatas = CollectModelData(gameWorld);
			context.lightData = CollectLightData(gameWorld);

			return context;
		}

	private:

		std::vector<CameraData> CollectCameraData(World& world)const;
		std::vector<ModelData> CollectModelData(World& world)const;
		LightData CollectLightData(World& world)const;

	public:

		FrameBuffer* GameFrameBuffer = nullptr;
		RenderContext GameContext;
		ClearParams GameClearParams;
	};
}