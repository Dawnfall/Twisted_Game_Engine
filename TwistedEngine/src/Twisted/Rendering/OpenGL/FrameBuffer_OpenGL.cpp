#include "Twisted/Rendering/FrameBuffer.h"

#include <glad/glad.h>
#include <string>
#include "Debug/Logger.h"

namespace Twisted
{

	void FrameBuffer::Init()
	{
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_tex->GetWidth(), m_tex->GetHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::string errorMessage = "ERROR::FRAMEBUFFER:: Framebuffer is not complete! Status: " + std::to_string(status);
			TWISTED_WARN(errorMessage);
		}

		if (m_tex) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex->GetTexID(), 0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_isDirty = true;
	}

	void FrameBuffer::Bind()
	{
		if (!IsValid())
			return; // nothing to bind

		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		if (IsDirty())
			Update();

		glViewport(0, 0, m_tex->GetWidth(),m_tex->GetHeight());
	}

	void FrameBuffer::UnBind()const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Destroy()
	{
		glDeleteFramebuffers(1, &m_id);
		glDeleteRenderbuffers(1, &m_rbo);

		SetTexture(nullptr);
		m_rbo = m_id = 0;
		m_isDirty = true;
	}

	void FrameBuffer::Blit(unsigned int destID)const
	{
		if (!IsValid())
			return;

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);      // source

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destID);

		glBlitFramebuffer(
			0, 0, m_tex->GetWidth(), m_tex->GetHeight(),   // src rect
			0, 0, m_tex->GetWidth(), m_tex->GetHeight(),   // dst rect
			GL_COLOR_BUFFER_BIT,   // what to copy
			GL_NEAREST             // filtering
		);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void FrameBuffer::ClearBuffers()const
	{
		if (m_clearBits)
		{
			if (m_params.doClearColor)
				glClearColor(m_params.clearColor.r, m_params.clearColor.g, m_params.clearColor.b, m_params.clearColor.a);
			if (m_params.doClearDepth)
				glClearDepth(1.0f);
			if (m_params.doClearStencil)
				glClearStencil(0);
			glClear(m_clearBits);
		}
	}

	void FrameBuffer::Update()
	{
		if (m_tex)
		{
			glBindTexture(GL_TEXTURE_2D, m_tex->GetTexID());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_tex->GetWidth(),m_tex->GetHeight(),
				0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}

		if (m_rbo)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
				m_tex->GetWidth(), m_tex->GetHeight());
		}

		if (m_params.doDepthTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		if (m_params.doClearStencil)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);

		glDepthMask(m_params.doDepthWrite ? GL_TRUE : GL_FALSE);

		GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		// Precompute clear bits
		m_clearBits = 0;
		if (m_params.doClearColor)   m_clearBits |= GL_COLOR_BUFFER_BIT;
		if (m_params.doClearDepth)   m_clearBits |= GL_DEPTH_BUFFER_BIT;
		if (m_params.doClearStencil) m_clearBits |= GL_STENCIL_BUFFER_BIT;

		m_isDirty = false;
	}
}