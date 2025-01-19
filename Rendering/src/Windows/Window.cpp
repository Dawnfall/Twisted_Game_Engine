#include "renderingpch.h"
#include "Window.h"
#include "Rendering/RenderingAPI.h"
namespace Twisted
{
	Window::Window(GLFWwindow* window, const std::string& title, Vec2i size, Vec2i position) :
		m_pointer(window),
		m_title(title),
		m_pos(position),
		m_size(size)
	{
	}

	void Window::SetCallbacks()
	{
		glfwSetWindowCloseCallback(m_pointer,
			[](GLFWwindow* pointer)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				window->CloseWindowEvent.Invoke();
			});
		glfwSetWindowSizeCallback(m_pointer,
			[](GLFWwindow* pointer, int newWidth, int newHeight)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				window->SetSize(Vec2i(newWidth, newHeight));
				window->WindowResizeEvent.Invoke();
			});
		glfwSetKeyCallback(m_pointer,
			[](GLFWwindow* pointer, int key, int scancode, int action, int mods)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				window->GetInput().KeyCallback(pointer, key, scancode, action, mods);
				window->KeyCallbackEvent.Invoke(key);
			});
		glfwSetMouseButtonCallback(m_pointer,
			[](GLFWwindow* pointer, int button, int action, int mods)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				window->GetInput().MouseButtonCallback(pointer, button, action, mods);
				window->ButtonCallbackEvent.Invoke(button);
			});
	}
	void Window::ClearWindow(Colors::Color color)const
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::SetTitle(const std::string& newTitle)
	{
		m_title = newTitle;
		glfwSetWindowTitle(m_pointer, m_title.c_str());
	}
	void Window::SetSize(Vec2i newSize)
	{
		m_size = newSize;
		glfwSetWindowSize(m_pointer, m_size.x, m_size.y);
	}
	void Window::SetPosition(Vec2i newPosition)
	{
		m_pos = newPosition;
		glfwSetWindowPos(m_pointer, m_pos.x, m_pos.y);
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_pointer);
	}
	void Window::PollEvents()
	{
		glfwPollEvents();
	}
	void Window::SetFullscreen(SRef<Monitor> monitor)
	{
		m_pos.x = 0;
		m_pos.y = 0;
		m_size.x = monitor->Resolution.x;
		m_size.y = monitor->Resolution.y;
		glfwSetWindowMonitor(m_pointer, monitor->Pointer, m_pos.x, m_pos.y, m_size.x, m_size.y, monitor->RefreshRate);
	}
	void Window::SetWindowed(Vec2i size, Vec2i pos)
	{
		m_pos = pos;
		m_size = size;
		glfwSetWindowMonitor(m_pointer, nullptr, m_pos.x, m_pos.y, m_size.x, m_size.y, GLFW_DONT_CARE);
	}

	void Window::SetViewPort(float width, float height)
	{
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	}
	void Window::SetVsync(int deltaFrames)
	{
		glfwSwapInterval(deltaFrames);
	}


	bool Window::CloseWindow()
	{
		glfwSetWindowShouldClose(m_pointer, GLFW_TRUE);
		//glfwDestroyWindow(m_window);

		TWISTED_INFO("Window closed");
		return true;
	}
	SRef<Window> Window::CreateNewWindow(const std::string& title, SRef<Monitor> monitor, Vec2i size, Vec2i position)
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

		if (!monitor)
			glfwSetWindowPos(windowPointer, position.x, position.y);

		SRef<Window> newWindow = std::make_shared<Window>(windowPointer, title, size, position);

		glfwSetWindowUserPointer(newWindow->m_pointer, newWindow.get());
		glfwMakeContextCurrent(newWindow->m_pointer);
		newWindow->SetCallbacks();
		RenderAPI::InitOpenGL((GLADloadproc)glfwGetProcAddress);

		TWISTED_INFO("Window Created");
		return newWindow;
	}
}