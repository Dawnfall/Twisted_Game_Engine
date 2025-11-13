#pragma once
#include "Twisted/Application/Layer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "Twisted/Rendering/Data/RenderContext.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "Data/Color.h"

#include <vector>

namespace Twisted
{
	struct ClearParams
	{
		bool doDepthTest = false;
		bool doDepthWrite = false;
		bool doDepthClear = false;
		bool doClearStencil = false;
		bool doClearColor = false;

		Color clearColor{ 0,0,0,1 };
	};

	class TWISTED_API RenderLayer :public Layer
	{
	public:
		RenderLayer(Application* app) :Layer(app)
		{
			s_instance = this;
		}

		static RenderLayer* GetInstance() { return s_instance; }

		void Render();

		void SubmitEntry(RenderContext&& newContext)
		{
			m_context = newContext;
			isUsed = false;
		}

		void Clear()
		{
			isUsed = true;
		}

		FrameBuffer* CreateFrameBuffer(const std::string& name, Vec2i size)
		{
			auto it = m_framebuffers.find(name);
			if (it != m_framebuffers.end())
				return it->second;

			FrameBuffer* newFrameBuffer = TObject::Create<FrameBuffer>(name, size);

			m_framebuffers[name] = newFrameBuffer;
			return newFrameBuffer;
		}
		void DestroyFrameBuffer(const std::string& name)
		{
			auto it = m_framebuffers.find(name);
			if (it != m_framebuffers.end())
			{
				TObject::Destroy(it->second);
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
		inline static RenderLayer* s_instance = nullptr;

		std::unordered_map<std::string, FrameBuffer*> m_framebuffers;

		bool isUsed = true;
		RenderContext m_context;

		unsigned int m_clearBits = 0;
		ClearParams m_clearParams;

		//void SetClearColor(const Color& color)
//{
//	if (m_params.clearColor != color)
//	{
//		m_params.clearColor = color;
//	}
//}
//void SetDepthTest(bool enabled)
//{
//	if (m_params.doDepthTest != enabled)
//	{
//		m_params.doDepthTest = enabled;
//		m_isDirty = true;
//	}
//}
//void SetDepthWrite(bool enabled)
//{
//	if (m_params.doDepthWrite != enabled)
//	{
//		m_params.doDepthWrite = enabled;
//		m_isDirty = true;
//	}
//}
//void SetClearDepth(bool enabled)
//{
//	if (m_params.doClearDepth != enabled)
//	{
//		m_params.doClearDepth = enabled;
//		m_isDirty = true;
//	}
//}
//void SetClearStencil(bool enabled)
//{
//	if (m_params.doClearStencil != enabled)
//	{
//		m_params.doClearStencil = enabled;
//		m_isDirty = true;
//	}
//}
//void SetClearColorFlag(bool enabled)
//{
//	if (m_params.doClearColor != enabled)
//	{
//		m_params.doClearColor = enabled;
//		m_isDirty = true;
//	}
//}
		void SetClearParams();
		void ApplyClearParams();
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


