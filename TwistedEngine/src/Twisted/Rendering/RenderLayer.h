#pragma once
#include "Twisted/Application/Layer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "Twisted/Rendering/Data/RenderSystemEntry.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "Twisted/ObjectManager.h"
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
			Render();
			m_entries.emplace_back(entry);
		}


		void Clear()
		{
			m_entries.clear();
		}

		FrameBuffer* CreateFrameBuffer(const std::string& name, Vec2i size)
		{
			auto it = m_framebuffers.find(name);
			if (it != m_framebuffers.end())
				return it->second;

			FrameBuffer* newFrameBuffer = ObjectManager::Create<FrameBuffer>(name, size);

			m_framebuffers[name] = newFrameBuffer;
			return newFrameBuffer;
		}
		void DestroyFrameBuffer(const std::string& name)
		{
			auto it = m_framebuffers.find(name);
			if (it != m_framebuffers.end())
			{
				ObjectManager::Destroy(it->second);
				m_framebuffers.erase(it);
			}
		}
		FrameBuffer* GetFrameBuffer(const std::string& name)
		{
			auto it = m_framebuffers.find(name);
			if (it != m_framebuffers.end())
				return it->second;
			return nullptr;
		}
	private:
		std::unordered_map<std::string, FrameBuffer*> m_framebuffers;
		std::vector<RenderSystemEntry> m_entries;

		void Render();

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
