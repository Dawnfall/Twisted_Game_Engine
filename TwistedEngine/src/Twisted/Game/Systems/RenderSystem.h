#pragma once

#include "AppCore.h"
#include "Twisted/Game/SystemBase.h"
#include <vector>


namespace Twisted::Render
{
	class RenderContext;
}

namespace Twisted
{
	namespace rend = Twisted::Render;

	class TWISTED_API RenderSystem :public SystemBase
	{
	public:
		RenderSystem(World* world) :SystemBase(world) {}
		void Update() override;

		void RegisterContext(rend::RenderContext* context);
		void UnregisterContext(rend::RenderContext* context);
	private:
		std::vector<rend::RenderContext*> m_contexts;
	};
}