#pragma once

#include "Twisted/Data/Color.h"
#include "Utils/GlmUtils.h"

namespace Twisted
{
	class TWISTED_API FrameBuffer
	{
	public:
		FrameBuffer(unsigned int width, unsigned int height);
		~FrameBuffer();

		void Resize(int width, int height);
		void ClearBuffers(bool doClearColor, Color clearColor, bool doClearDepth, bool doClearStencil)const;

		void SetEnableDepthTest(bool doEnable)const;
		void SetEnableDepthWrite(bool doEnable)const;
		void SetEnableStencilTest(bool doEnable)const;

		unsigned int GetTexID()const { return m_tex; }
		Vec2i GetSize()const { return m_size; }

		void Blit(unsigned int destID) const;
		void Bind()const;
		void UnBind()const;

	private:
		Vec2i m_size;
	
		unsigned int m_id;
		unsigned int m_tex;
		unsigned int m_rbo;
	};
}