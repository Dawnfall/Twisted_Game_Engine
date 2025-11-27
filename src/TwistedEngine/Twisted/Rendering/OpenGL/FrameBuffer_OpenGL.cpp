
#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
#include "Twisted/Rendering/FrameBuffer.h"

#include <glad/glad.h>
#include <string>
#include "Debug/Logger.h"

#include "Twisted/Rendering/OpenGL/Texture_OpenGL.h"


namespace Twisted
{
	FrameBuffer::FrameBuffer(const std::string& name, Vec2i size) :
		TObject(name),
		Size(size)
	{
	}

	void FrameBuffer::OnCreate()
	{
		FrameBuffer_GL::Init(*this);
	}

	void FrameBuffer::OnDestroy()
	{
		FrameBuffer_GL::Destroy(*this);
	}
}

namespace Twisted::FrameBuffer_GL
{
	void Init(FrameBuffer& framebuffer)
	{
		framebuffer.Tex = TObject::Create<Texture>("_mainTex");
		Texture_GL::Resize(*framebuffer.Tex, framebuffer.Size);

		if (!framebuffer.Tex || !framebuffer.Tex->IsValid())
		{
			TWISTED_WARN("Framebuffer without texture: {}", framebuffer.GetName());
			return;
		}

		glGenFramebuffers(1, &framebuffer.Id);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.Id);

		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			framebuffer.Tex->TexID,
			0
		);

		glGenRenderbuffers(1, &framebuffer.Rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, framebuffer.Rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer.Tex->Width, framebuffer.Tex->Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer.Rbo);

		GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::string errorMessage = "ERROR::FRAMEBUFFER:: Framebuffer is not complete! Status: " + std::to_string(status);
			TWISTED_WARN(errorMessage);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		framebuffer.IsDirty = true;

		TWISTED_INFO("Framebuffer '{}' created successfully ({}x{})", framebuffer.GetName(), framebuffer.Size.x, framebuffer.Size.y);
	}
	void Destroy(FrameBuffer& framebuffer)
	{
		glDeleteFramebuffers(1, &framebuffer.Id);
		glDeleteRenderbuffers(1, &framebuffer.Rbo);

		TObject::Destroy(framebuffer.Tex.GetObj());
		framebuffer.Rbo = framebuffer.Id = 0;
		framebuffer.IsDirty = true;
	}


	void Bind(FrameBuffer& framebuffer)
	{
		if (!framebuffer.IsValid())
			return; // nothing to bind

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.Id);

		if (framebuffer.IsDirty)
		{
			FrameBuffer_GL::Update(framebuffer);
		}

		glViewport(0, 0, framebuffer.Tex->Width, framebuffer.Tex->Height);
	}

	void UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Update(FrameBuffer& framebuffer)
	{
		if (framebuffer.Tex)
		{
			glBindTexture(GL_TEXTURE_2D, framebuffer.Tex->TexID);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA8,
				framebuffer.Tex->Width,
				framebuffer.Tex->Height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				nullptr
			);
		}

		if (framebuffer.Rbo)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, framebuffer.Rbo);
			glRenderbufferStorage(
				GL_RENDERBUFFER,
				GL_DEPTH24_STENCIL8,
				framebuffer.Tex->Width,
				framebuffer.Tex->Height
			);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		framebuffer.IsDirty = false;
	}

	void Blit(FrameBuffer& framebuffer, unsigned int destID) //TODO... may be improved
	{
		if (!framebuffer.IsValid())
			return;

		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer.Id);      // source

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destID);

		glBlitFramebuffer(
			0, 0, framebuffer.Tex->Width, framebuffer.Tex->Height,   // src rect
			0, 0, framebuffer.Tex->Width, framebuffer.Tex->Height,   // dst rect
			GL_COLOR_BUFFER_BIT,   // what to copy
			GL_NEAREST             // filtering
		);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void SetSize(FrameBuffer& framebuffer, const Vec2i& size)
	{
		if (framebuffer.Tex && framebuffer.Tex->IsValid())
		{
			if (framebuffer.Size != size)
			{
				framebuffer.Size = size;
				Texture_GL::Resize(*framebuffer.Tex, framebuffer.Size);
				framebuffer.IsDirty = true;
			}
		}
		else
			TWISTED_WARN("Cannot resize framebuffer with invalid texture");
	}

	void ClearBuffer(const ClearParams& clearParams) //assumes bound buffer
	{
		int clearBits = 0;
		if (clearParams.doClearColor)
		{
			glClearColor(
				clearParams.clearColor.r,
				clearParams.clearColor.g,
				clearParams.clearColor.b,
				clearParams.clearColor.a
			);
			clearBits |= GL_COLOR_BUFFER_BIT;
		}
		if (clearParams.doDepthClear)
		{
			glClearDepth(1.0f);
			clearBits |= GL_DEPTH_BUFFER_BIT;
		}
		if (clearParams.doClearStencil)
		{
			glClearStencil(0);
			clearBits |= GL_STENCIL_BUFFER_BIT;
		}

		if (clearBits != 0)
			glClear(clearBits);
	}

}

