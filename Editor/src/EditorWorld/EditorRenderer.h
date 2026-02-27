#pragma once
#include "AppCore.h"

#include "Twisted/Gameing/World.h"
#include "Twisted/Rendering/Data/ClearParams.h"
#include "Twisted/Rendering/Data/RenderContext.h"
#include "Twisted/Rendering/RenderAPI.h"
#include <vector>

namespace Twisted::Editor
{
	class EditorRenderer
	{
	public:
		void Render(World& editorWorld, World& gameWorld)const;
		RenderContext ExtractContext(World& editorWorld, World& gameWorld)const;

		ClearParams m_clearParams;
		RenderContext m_context;
		unsigned int m_clearBits = 0;

	private:
		std::vector<CameraData> CollectCameraData(World& editorWorld, World& world)const;
		std::vector<ModelData> CollectModelData(World& editorWorld, World& world)const;
		LightData CollectLightData(World& editorWorld, World& world)const;
	};
}