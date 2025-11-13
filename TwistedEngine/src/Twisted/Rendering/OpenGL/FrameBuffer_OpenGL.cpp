#include "Twisted/Rendering/FrameBuffer.h"

#include <glad/glad.h>
#include <string>
#include "Debug/Logger.h"

namespace Twisted
{
	void FrameBuffer::OnCreate()
	{
		m_tex = TObject::Create<Texture>("_mainTex");
		m_tex->Resize(m_size);

		if (!m_tex || !m_tex->IsValid())
		{
			TWISTED_WARN("Framebuffer without texture: {}", GetName());
			return;
		}

		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			m_tex->GetTexID(),
			0
		);

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_tex->GetWidth(), m_tex->GetHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::string errorMessage = "ERROR::FRAMEBUFFER:: Framebuffer is not complete! Status: " + std::to_string(status);
			TWISTED_WARN(errorMessage);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		m_isDirty = true;

		TWISTED_INFO("Framebuffer '{}' created successfully ({}x{})", GetName(), m_size.x, m_size.y);
	}

	void FrameBuffer::SetSize(const Vec2i& size)
	{
		if (m_tex && m_tex->IsValid())
		{
			if (m_size != size)
			{
				m_size = size;
				m_tex->Resize(m_size);
				m_isDirty = true;
			}
		}
		else
			TWISTED_WARN("Cannot resize framebuffer with invalid texture");
	}

	void FrameBuffer::Bind()
	{
		if (!IsValid())
			return; // nothing to bind

		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		if (m_isDirty)
		{
			Update();
		}

		glViewport(0, 0, m_tex->GetWidth(), m_tex->GetHeight());
	}

	void FrameBuffer::UnBind()const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Destroy()
	{
		glDeleteFramebuffers(1, &m_id);
		glDeleteRenderbuffers(1, &m_rbo);

		TObject::Destroy(m_tex.GetObj());
		m_rbo = m_id = 0;
		m_isDirty = true;
	}

	void FrameBuffer::Blit(unsigned int destID)const //TODO... may be improved
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

	void FrameBuffer::Update()
	{
		if (m_tex)
		{
			glBindTexture(GL_TEXTURE_2D, m_tex->GetTexID());
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA8,
				m_tex->GetWidth(),
				m_tex->GetHeight(),
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				nullptr
			);
		}

		if (m_rbo)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
			glRenderbufferStorage(
				GL_RENDERBUFFER,
				GL_DEPTH24_STENCIL8,
				m_tex->GetWidth(),
				m_tex->GetHeight()
			);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		m_isDirty = false;
	}
}

