#include "Twisted/Rendering/RenderService.h"
#include "Twisted/Rendering/RenderAPI.h"

namespace Twisted
{
	RenderService::RenderService(Application* app):Service(app) {};

	void RenderService::ForwardRender(const RenderContext& renderContext)
	{
		Twisted::Render::ForwardRender(renderContext);
	}
}