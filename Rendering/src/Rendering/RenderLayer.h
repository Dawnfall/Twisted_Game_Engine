#pragma once
#include "Twisted/Application/Layer.h"
#include "RenderingAPI.h"

#include "Rendering/RenderContext.h"

#include <unordered_map>
#include <string>

namespace Twisted::Render
{
	class RenderLayer :public Layer
	{
	public:
		RenderLayer(Application* app) :Layer(app)
		{
		}

		void InitWindowContext(void* context)
		{
			Render::InitRenderer(context);
		}

		Twisted::Render::RenderContext* GetContext(const std::string& name)
		{
			//auto res = std::find(m_contexts.begin(), m_contexts.end(), name);
			//if (res != m_contexts.end())
			//	return &res->second;
			return nullptr;
		}
		Twisted::Render::RenderContext* CreateNew(const std::string& name,unsigned int width,unsigned int height)
		{
			m_contexts.emplace(name, Twisted::Render::RenderContext(width, height));
			return &m_contexts.at(name);
		}
		bool RemoveContext(const std::string& name)
		{
			return m_contexts.erase(name) > 0;
		}

	private:
		std::unordered_map<std::string, Twisted::Render::RenderContext> m_contexts;
	};


}