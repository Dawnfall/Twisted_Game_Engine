#ifdef TWISTED_BACKEND_OPENGL

#include "OpenGL/FrameBuffer_OpenGL.h"
#include "FrameBuffer.h"
#include "OpenGL/Texture_OpenGL.h"
#include "Application/TObject.h"
#include "Debug/Logger.h"

#include <glad/glad.h>

namespace Twisted
{
	Framebuffer::Framebuffer(const std::string& name) :
		TObject(name),
		m_backend(std::make_unique<FramebufferBackend>())
	{
	}

	Framebuffer::~Framebuffer()
	{
	}

	void Framebuffer::OnCreate()
	{
		// Set default descriptors
		Color.OwnedDesc.Format     = TextureFormat::RGBA8;
		Color.OwnedDesc.UsageFlags = TextureUsage_ColorAttachment | TextureUsage_Sampled;

		Depth.OwnedDesc.Format     = TextureFormat::Depth24Stencil8;
		Depth.OwnedDesc.UsageFlags = TextureUsage_DepthAttachment;

		// Create engine texture objects
		Color.Owned = WPtr<Texture>(TObject::Create<Texture>(GetName() + "_color"));
		Depth.Owned = WPtr<Texture>(TObject::Create<Texture>(GetName() + "_depth"));

		Color.Owned->Info = Color.OwnedDesc;
		Depth.Owned->Info = Depth.OwnedDesc;

		m_backend = std::make_unique<FramebufferBackend>();
		glCreateFramebuffers(1, &m_backend->Id);
		// Create the GL FBO handle (no attachments yet — size not known)
	}

	void Framebuffer::OnDestroy()
	{
		TObject::Destroy(Color.Owned.get());
		TObject::Destroy(Depth.Owned.get());
		Color.Owned = nullptr;
		Depth.Owned = nullptr;

		glDeleteFramebuffers(1, &m_backend->Id);
		m_backend = nullptr;
	}

	void Framebuffer::SetSize(const Vec2i& newSize)
	{
		if (newSize.x <= 0 || newSize.y <= 0)
			return;
		if (newSize == Size && m_backend->IsValid())
			return;

		Size = newSize;

		// Resize owned attachments
		if (Texture* colorTex = Color.Owned.get())
			colorTex->Resize(newSize);
		if (Texture* depthTex = Depth.Owned.get())
			depthTex->Resize(newSize);

		// Attach to FBO
		GLuint colorID = Color.Owned->GetBackend()->TexID;
		GLuint depthID = Depth.Owned->GetBackend()->TexID;

		glNamedFramebufferTexture(m_backend->Id, GL_COLOR_ATTACHMENT0,    colorID, 0);
		glNamedFramebufferTexture(m_backend->Id, GL_DEPTH_STENCIL_ATTACHMENT, depthID, 0);

		constexpr GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glNamedFramebufferDrawBuffers(m_backend->Id, 1, drawBuffers);

		GLenum status = glCheckNamedFramebufferStatus(m_backend->Id, GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			TWISTED_WARN("Framebuffer '{}' incomplete, status: {:#x}", GetName(), status);

		Version++;
	}

	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_backend->Id);
		glViewport(0, 0, Size.x, Size.y);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

namespace Twisted::GL
{

	void ClearBuffer(const ClearParams& clearParams)
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
