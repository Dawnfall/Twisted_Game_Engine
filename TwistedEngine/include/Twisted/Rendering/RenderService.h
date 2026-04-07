#pragma once
#include "AppCore.h"
#include "Twisted/Application/Service.h"
#include "Twisted/Rendering/RenderConvert.h"

namespace Twisted
{
	class TWISTED_API RenderService :public Service
	{
	public:
		RenderService(Application* app);

		void ForwardRender(const RenderContext& renderContext);
	};
}
