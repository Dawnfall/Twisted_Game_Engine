#pragma once

#include <glad/glad.h>
#include "Utils/Color.h"
#include "Utils/GlmUtils.h"

namespace Twisted
{
	class FrameBuffer
	{
	public:
		FrameBuffer(unsigned int width, unsigned int height);
		~FrameBuffer();

		void Resize(GLsizei width, GLsizei height);
		void ClearBuffers(bool doClearColor, Colors::Color clearColor, bool doClearDepth, bool doClearStencil)const;

		void SetEnableDepthTest(bool doEnable)const;
		void SetEnableDepthWrite(bool doEnable)const;
		void SetEnableStencilTest(bool doEnable)const;

		GLuint GetTexID()const { return m_tex; }
		Vec2i GetSize()const { return m_size; }

		void Bind()const;
		void UnBind()const;

	private:
		Vec2i m_size;
	
		GLuint m_id;
		GLuint m_tex;
		GLuint m_rbo;
	};
}