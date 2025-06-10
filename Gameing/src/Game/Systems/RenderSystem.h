#pragma once

#include "AppCore.h"
#include "Game/SystemBase.h"
#include "Game/Systems/RenderSystemEntry.h"
#include "Rendering/RenderContext.h"

#include <vector>
#include <unordered_map>

namespace Twisted
{
	namespace rend = Twisted::Render;

	class TWISTED_API RenderSystem :public SystemBase
	{
	public:
		RenderSystem(World* world) :SystemBase(world) {}
		void Update() override;

		void AddEntry(rend::RenderContext* renderContext, EntityID camera)
		{
			RenderSystemEntry newEntry{ renderContext,camera };
			m_entries.emplace_back(newEntry);
		}
		void RemoveEntry();
	private:
		std::vector<RenderSystemEntry> m_entries;
	};
}