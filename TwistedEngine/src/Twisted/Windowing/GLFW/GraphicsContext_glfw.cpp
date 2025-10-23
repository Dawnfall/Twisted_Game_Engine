#include "AppCore.h"
#ifndef NATIVE_USE

#include "Twisted/Windowing/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace Twisted
{
	GraphicsContext::GraphicsContext(void* windowHandle) :m_windowHandle(windowHandle)
	{

	}
	GraphicsContext::~GraphicsContext()
	{
		
	}

	void GraphicsContext::SetVSync(int deltaFrames)
	{
		glfwSwapInterval(deltaFrames);
	}

	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(m_windowHandle));
	}

	void GraphicsContext::Clear(const Color& color)
	{
		//Render::FrameBuffer::ClearBuffers();
		//glClearColor(color.r, color.g, color.b, color.a);
		//glClear(GL_COLOR_BUFFER_BIT);
	}
}
#endif