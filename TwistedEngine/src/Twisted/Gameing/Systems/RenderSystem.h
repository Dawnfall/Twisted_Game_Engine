#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/SystemBase.h"
#include "Twisted/Gameing/Systems/RenderSystemEntry.h"
#include "Twisted/Rendering/RenderContext.h"

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