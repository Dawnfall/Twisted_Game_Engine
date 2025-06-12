
#include "Twisted/Rendering/FrameBuffer.h"

#include <glad/glad.h>
#include <string>
#include "Logger.h"

namespace Twisted::Render
{
	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) :
		m_size(width, height)
	{
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		glGenTextures(1, &m_tex);
		glBindTexture(GL_TEXTURE_2D, m_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::string errorMessage = "ERROR::FRAMEBUFFER:: Framebuffer is not complete! Status: " + std::to_string(status);
			TWISTED_WARN(errorMessage);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_id);
		glDeleteTextures(1, &m_tex);
		glDeleteRenderbuffers(1, &m_rbo);
	}

	void FrameBuffer::Resize(int width, int height)
	{
		m_size.x = width;
		m_size.y = height;

		glBindTexture(GL_TEXTURE_2D, m_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}

	void FrameBuffer::Blit(unsigned int destID)const
	{
		// Bind the source framebuffer for reading
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
		// Bind the destination framebuffer for drawing
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destID);

		// Blit the color buffer from source to destination
		glBlitFramebuffer(
			0, 0, m_size.x, m_size.y,   // src rect
			0, 0, m_size.x, m_size.y,   // dst rect
			GL_COLOR_BUFFER_BIT,   // what to copy
			GL_NEAREST             // filtering
		);
	}


	void FrameBuffer::ClearBuffers(bool doClearColor, Color clearColor, bool doClearDepth, bool doClearStencil)const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		int bufferBits = 0;
		if (doClearColor)
		{
			bufferBits |= GL_COLOR_BUFFER_BIT;
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		}
		if (glClearStencil)
			bufferBits |= GL_STENCIL_BUFFER_BIT;
		if (doClearDepth)
			bufferBits |= GL_DEPTH_BUFFER_BIT;

		glClear(bufferBits);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the framebuffer
	}

	void FrameBuffer::Bind()const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	void FrameBuffer::UnBind()const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}