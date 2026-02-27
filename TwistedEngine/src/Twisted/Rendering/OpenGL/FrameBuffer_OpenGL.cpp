#ifndef TWISTED_D3D

#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/Rendering/OpenGL/Texture_OpenGL.h"
#include "Debug/Logger.h"

#include <glad/glad.h>
#include <string>

namespace Twisted
{
	Framebuffer::Framebuffer(const std::string& name) :
		TObject(name)
	{
		m_backend = new FramebufferBackend();
	}

	Framebuffer::~Framebuffer()
	{
		delete m_backend;
	}

	void Framebuffer::SetSize(const Vec2i& size)
	{
		//if (Tex && framebuffer.Tex->IsValid())
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

	void Framebuffer::OnCreate()
	{
		Color.Owned = TObject::Create<Texture>("color");
		Depth.Owned = TObject::Create<Texture>("depth");

		//glGenTextures(1, &m_backend->TexID);    

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_backend->TexID);

		//glTexImage2D(
		//	GL_TEXTURE_2D,
		//	0,
		//	GL_RGBA8,
		//	Size.x,
		//	Size.y,
		//	0,
		//	GL_RGBA,
		//	GL_UNSIGNED_BYTE,
		//	0);

		////apply params

		//glBindTexture(GL_TEXTURE_2D, 0);

		//if (m_backend->TexID == 0)
		//{
		//	TWISTED_WARN("Framebuffer without texture: {}", GetName());
		//	return;
		//}

		//glGenFramebuffers(1, &m_backend->Id);
		//glBindFramebuffer(GL_FRAMEBUFFER, m_backend->Id);

		//glFramebufferTexture2D(
		//	GL_FRAMEBUFFER,
		//	GL_COLOR_ATTACHMENT0,
		//	GL_TEXTURE_2D,
		//	m_backend->TexID,
		//	0
		//);

		//glGenRenderbuffers(1, &m_backend->Rbo);
		//glBindRenderbuffer(GL_RENDERBUFFER, m_backend->Rbo);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Size.x, Size.y);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_backend->Rbo);

		//GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		//glDrawBuffers(1, drawBuffers);

		//GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		//if (status != GL_FRAMEBUFFER_COMPLETE) {
		//	std::string errorMessage = "ERROR::FRAMEBUFFER:: Framebuffer is not complete! Status: " + std::to_string(status);
		//	TWISTED_WARN(errorMessage);
		//}

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//IsDirty = true;

		//TWISTED_INFO("Framebuffer '{}' created successfully ({}x{})", GetName(), Size.x, Size.y);
	}


	void Framebuffer::OnDestroy()
	{
		TObject::Destroy(Color.Owned.get());
		TObject::Destroy(Depth.Owned.get());
	}

	//void Framebuffer::Update()
	//{
		//if (m_backend->TexID)
		//{
		//	glBindTexture(GL_TEXTURE_2D, m_backend->TexID);
		//	glTexImage2D(
		//		GL_TEXTURE_2D,
		//		0,
		//		GL_RGBA8,
		//		Size.x,
		//		Size.y,
		//		0,
		//		GL_RGBA,
		//		GL_UNSIGNED_BYTE,
		//		nullptr
		//	);
		//}

		//if (m_backend->Rbo)
		//{
		//	glBindRenderbuffer(GL_RENDERBUFFER, m_backend->Rbo);
		//	glRenderbufferStorage(
		//		GL_RENDERBUFFER,
		//		GL_DEPTH24_STENCIL8,
		//		Size.x,
		//		Size.y
		//	);
		//}

		//glBindTexture(GL_TEXTURE_2D, 0);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//IsDirty = false;
	//}



}

namespace Twisted::GL
{
	void Blit(FramebufferBackend& framebuffer, GLuint destID) //TODO... may be improved
	{
		//if (!framebuffer.IsValid())
		//	return;

		//glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer.Id);      // source

		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destID);

		//glBlitFramebuffer(
		//	0, 0, framebuffer.Tex->GetWidth(), framebuffer.Tex->GetHeight(),   // src rect
		//	0, 0, framebuffer.Tex->GetWidth(), framebuffer.Tex->GetHeight(),   // dst rect
		//	GL_COLOR_BUFFER_BIT,   // what to copy
		//	GL_NEAREST             // filtering
		//);

		//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
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