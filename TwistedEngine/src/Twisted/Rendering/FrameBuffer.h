#pragma once

#include "AppCore.h"

#include "Twisted/TObject.h"
#include "Data/Color.h"
#include "Utils/GlmUtils.h"
#include "Texture.h"
#include "Utils/WPtr.h"
#include "Twisted/ObjectManager.h"

namespace Twisted
{
	struct FrameBufferParams
	{
		bool doDepthTest = false;
		bool doDepthWrite = false;
		bool doClearDepth = false;
		bool doClearStencil = false;
		bool doClearColor = false;

		Color clearColor{ 0,0,0,1 };
	};

	class TWISTED_API FrameBuffer :public TObject
	{
	public:
		FrameBuffer(const std::string& name, Vec2i size):
			TObject(name),
			m_size(size)
		{ }
			
		FrameBuffer(const std::string& name,Vec2i size,const FrameBufferParams& params):
			TObject(name),
			m_size(size),
			m_params(params)
		{ }

		void OnCreate()override
		{
			//m_tex = ObjectManager::Create<Texture>("main");
		}

		void OnDestroy() override
		{
			Destroy();
		}
		
		bool IsValid()const 
		{ 
			return m_id != 0; 
		}
		bool IsDirty()const { return m_isDirty; }
		void SetClearColor(const Color& color)
		{
			if (m_params.clearColor != color)
			{
				m_params.clearColor = color;
			}
		}
		void SetDepthTest(bool enabled)
		{
			if (m_params.doDepthTest != enabled)
			{
				m_params.doDepthTest = enabled;
				m_isDirty = true;
			}
		}
		void SetDepthWrite(bool enabled)
		{
			if (m_params.doDepthWrite != enabled)
			{
				m_params.doDepthWrite = enabled;
				m_isDirty = true;
			}
		}
		void SetClearDepth(bool enabled)
		{
			m_params.doClearDepth = enabled;
		}
		void SetClearStencil(bool enabled)
		{
			m_params.doClearStencil = enabled;
		}
		void SetClearColorFlag(bool enabled)
		{
			m_params.doClearColor = enabled;
		}
		void SetParams(const FrameBufferParams& params)
		{
			m_params = params;
			m_isDirty = true;
		}
		void SetTexture(Texture* texture)
		{
			if (m_tex.get() == texture)
				return;
			m_tex = texture;
			m_isDirty = true;
		}

		void Init();
		void Destroy();

		void ClearBuffers()const;

		void SetSize(const Vec2i& size)
		{
			if (m_size != size)
			{
				m_size = size;
				m_tex->Resize(m_size);
				m_isDirty = true;
			}
		}

		Texture* GetTexture() { return m_tex.get(); }
		Vec2i GetSize()const { return m_size; }

		void Blit(unsigned int destID) const;
		void Bind();
		void UnBind()const;

		void Update();

	private:

		Vec2i m_size;
		FrameBufferParams m_params;

		WPtr<Texture> m_tex;
		unsigned int m_id = 0;
		unsigned int m_rbo = 0;

		unsigned int m_clearBits = 0;

		bool m_isDirty = true;
	};
}