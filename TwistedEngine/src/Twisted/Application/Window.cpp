#include "twistedpch.h"
#include "Window.h"
#include "Rendering/RenderingAPI.h"
#include <GLFW/glfw3.h>
#include <Utils/Color.h>

namespace rend = Twisted::Render;

namespace Twisted
{
	bool InitGLFW()
	{
		if (!glfwInit())
		{
			TWISTED_ERROR("GLFW init failure; RenderCore Init failure!");
			return false;
		}
		glfwSetErrorCallback([](int code, const char* description) {
			TWISTED_ERROR(description);
			});

		TWISTED_INFO("GLFW init success");
		TWISTED_INFO("RenderCore Init success!");

		return true;
	}
	void TerminateGLFW()
	{
		glfwTerminate();
	}

	Window::Window(GLFWwindow* window, const std::string& title, Vec2i size, Vec2i position) :
		m_pointer(window)
	{
		glfwSetWindowUserPointer(GetPointer(), this);
		glfwSetWindowPos(GetPointer(), position.x, position.y);
		//m_frameBuffer = std::make_unique<FrameBuffer>(size.x, size.y);

		glfwSetWindowCloseCallback(GetPointer(),
			[](GLFWwindow* pointer)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				window->CloseWindowEvent.Invoke();
			});
		glfwSetWindowSizeCallback(GetPointer(),
			[](GLFWwindow* pointer, int newWidth, int newHeight)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				//window->m_frameBuffer->Resize(newWidth, newHeight);
				window->WindowResizeEvent.Invoke();
			});
		glfwSetKeyCallback(GetPointer(),
			[](GLFWwindow* pointer, int key, int scancode, int action, int mods)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				//InputManager::GetInstance().KeyCallback(pointer, key, scancode, action, mods);
			});
		glfwSetMouseButtonCallback(GetPointer(),
			[](GLFWwindow* pointer, int button, int action, int mods)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				//InputManager::GetInstance().MouseButtonCallback(pointer, button, action, mods);
			});
	}

	Vec2i Window::GetSize()const
	{
		Vec2i size;
		glfwGetWindowSize(m_pointer, &size.x, &size.y);
		return size;
	}
	Vec2i Window::GetPosition()const
	{
		Vec2i pos;
		glfwGetWindowPos(m_pointer, &pos.x, &pos.y);
		return pos;
	}
	std::string Window::GetTitle()const
	{
		const char* title = glfwGetWindowTitle(m_pointer);
		return std::string(title);
	}

	void Window::SetTitle(const std::string& newTitle)
	{
		glfwSetWindowTitle(m_pointer, newTitle.c_str());
	}
	void Window::SetSize(Vec2i newSize)
	{
		glfwSetWindowSize(m_pointer, newSize.x, newSize.y);
	}
	void Window::SetPosition(Vec2i newPosition)
	{
		glfwSetWindowPos(m_pointer, newPosition.x, newPosition.y);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}
	void Window::SwapBuffers()
	{
		//m_frameBuffer->Blit(0);
		glfwSwapBuffers(m_pointer);
	}

	void Window::SetFullscreen(SRef<Monitor> monitor)
	{
		glfwSetWindowMonitor(m_pointer, monitor->Pointer, 100, 100, 300, 300, monitor->RefreshRate); //TODO...
	}

	void Window::SetWindowed(Vec2i size, Vec2i pos)
	{
		glfwSetWindowMonitor(m_pointer, nullptr, pos.x, pos.y, size.x, size.y, GLFW_DONT_CARE);
	}

	void Window::SetVsync(int deltaFrames)
	{
		glfwSwapInterval(deltaFrames);
	}

	void Window::Clear(const Color& color)
	{
		//Render::FrameBuffer::ClearBuffers();
		//glClearColor(color.r, color.g, color.b, color.a);
		//glClear(GL_COLOR_BUFFER_BIT);
	}

	bool Window::CloseWindow()
	{
		glfwSetWindowShouldClose(m_pointer, GLFW_TRUE);

		TWISTED_INFO("Window closed");
		return true;
	}

	URef<Window> Window::CreateNewWindow(const std::string& title, SRef<Monitor> monitor, Vec2i size, Vec2i position)
	{
		GLFWwindow* windowPointer = nullptr;
		if (monitor)
			windowPointer = glfwCreateWindow(monitor->Size.x, monitor->Size.y, title.c_str(), monitor->Pointer, NULL);
		else
			windowPointer = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, NULL);
		if (!windowPointer)
		{
			TWISTED_ERROR("CreateNewWindow() failure! GLFW window pointer creation failure");
			return nullptr;
		}
		glfwMakeContextCurrent(windowPointer);
		rend::InitRenderer(glfwGetProcAddress);

		TWISTED_INFO("Window Created");
		return std::make_unique<Window>(windowPointer, title, size, position);
	}

}