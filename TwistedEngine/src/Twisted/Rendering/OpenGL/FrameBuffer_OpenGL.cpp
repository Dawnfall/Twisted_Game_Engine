#ifndef TWISTED_D3D

#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/Rendering/OpenGL/Texture_OpenGL.h"
#include "Debug/Logger.h"

#include <glad/glad.h>
#include <string>
namespace Twisted
{
	FrameBuffer::FrameBuffer(const std::string& name, Vec2i size) :
		TObject(name)//,
		//Size(size)
	{
		(void)size;
	}
	

	void FrameBuffer::OnCreate()
	{
		//Init();
	}

	void FrameBuffer::OnDestroy()
	{
		//FrameBuffer_OpenGL::Destroy(*this);
	}

	void FrameBuffer::SetSize(const Vec2i& size)
	{
		//if (framebuffer.Tex && framebuffer.Tex->IsValid())
		//{
		//	if (framebuffer.Size != size)
		//	{
		//		framebuffer.Size = size;
		//		framebuffer.Tex->Resize(framebuffer.Size);
		//		framebuffer.IsDirty = true;
		//	}
		//}
		//else
		//	TWISTED_WARN("Cannot resize framebuffer with invalid texture");
	}
}

namespace Twisted::GL
{
	FrameBuffer_OpenGL::FrameBuffer_OpenGL()
	{
		Tex = std::make_unique<Texture_OpenGL>();
		Tex->Resize(Size);

		if (!Tex || !Tex->IsValid())
		{
			//TWISTED_WARN("Framebuffer without texture: {}", GetName());
			return;
		}

		glGenFramebuffers(1, &Id);
		glBindFramebuffer(GL_FRAMEBUFFER, Id);

		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			Tex->TexID,
			0
		);

		glGenRenderbuffers(1, &Rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, Rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Tex->Width, Tex->Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Rbo);

		GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::string errorMessage = "ERROR::FRAMEBUFFER:: Framebuffer is not complete! Status: " + std::to_string(status);
			TWISTED_WARN(errorMessage);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		IsDirty = true;

		//TWISTED_INFO("Framebuffer '{}' created successfully ({}x{})", GetName(), Size.x, Size.y);
	}

	FrameBuffer_OpenGL::~FrameBuffer_OpenGL()
	{
		glDeleteFramebuffers(1, &Id);
		glDeleteRenderbuffers(1, &Rbo);

		Rbo = Id = 0;
		IsDirty = true;
	}

	void FrameBuffer_OpenGL::Bind()
	{
		if (!IsValid())
			return; // nothing to bind

		glBindFramebuffer(GL_FRAMEBUFFER, Id);

		if (IsDirty)
		{
			Update();
		}

		glViewport(0, 0, Tex->Width, Tex->Height);
	}

	void FrameBuffer_OpenGL::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer_OpenGL::Update()
	{
		if (Tex)
		{
			glBindTexture(GL_TEXTURE_2D, Tex->TexID);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA8,
				Tex->Width,
				Tex->Height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				nullptr
			);
		}

		if (Rbo)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, Rbo);
			glRenderbufferStorage(
				GL_RENDERBUFFER,
				GL_DEPTH24_STENCIL8,
				Tex->Width,
				Tex->Height
			);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		IsDirty = false;
	}





	



	void Blit(FrameBuffer_OpenGL& framebuffer, unsigned int destID) //TODO... may be improved
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

#endif