#include "Twisted/Rendering/RenderContext.h"
#include "glad/glad.h"

namespace Twisted
{
	RenderContext::RenderContext(int w, int h) :
		frameBuffer(std::make_unique<FrameBuffer>(w, h))
	{

	}

	void RenderContext::Bind()
	{
		frameBuffer->Bind();

		if (doDepthTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		glDepthMask(doDepthWrite);

		if (doStencilTest)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
	}

	void RenderContext::UnBind()
	{
		frameBuffer->UnBind();
	}

	void RenderContext::Resize(int width, int height)
	{
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	}

	void RenderContext::Clear(Color color)const
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}