#pragma once
#include "Twisted/Application/Layer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "Twisted/Rendering/Data/RenderSystemEntry.h"
#include <vector>

namespace Twisted
{
	class TWISTED_API RenderLayer :public Layer
	{
	public:
		RenderLayer(Application* app) :Layer(app)
		{
		}
		void Update();
		void SubmitEntry(const RenderSystemEntry& entry)
		{
			m_entries.emplace_back(entry);
		}


		void Clear()
		{
			m_entries.clear();
		}

	private:
		
		void Render();

		std::vector<RenderSystemEntry> m_entries;
	};
}

//void InitWindowContext(void* context)
//{
//	InitRenderer(context);
//}
//
//RenderContext* GetContext(const std::string& name)
//{
//	//auto res = std::find(m_contexts.begin(), m_contexts.end(), name);
//	//if (res != m_contexts.end())
//	//	return &res->second;
//	return nullptr;
//}
//RenderContext* CreateNew(const std::string& name, unsigned int width, unsigned int height)
//{
//	m_contexts.emplace(name, RenderContext(width, height));
//	return &m_contexts.at(name);
//}
//bool RemoveContext(const std::string& name)
//{
//	return m_contexts.erase(name) > 0;
//}

//std::unordered_map<std::string, RenderContext> m_contexts;
