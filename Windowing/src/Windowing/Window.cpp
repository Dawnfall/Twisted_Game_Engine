#include "Window.h"
#include "Data/Color.h"
#include "MonitorInfo.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h> // Must be included after glfw3.h

namespace Twisted
{
	class Window::Impl
	{
	public:
		GLFWwindow* Pointer;
	};

	Window::Window(const std::string& title, Vec2i size, Vec2i position) :
		m_impl(new Impl())
	{
	}

	Window::~Window()
	{
		delete m_impl;
	}

	Vec2i Window::GetSize()const
	{
		Vec2i size;
		glfwGetWindowSize(m_impl->Pointer, &size.x, &size.y);
		return size;
	}
	Vec2i Window::GetPosition()const
	{
		Vec2i pos;
		glfwGetWindowPos(m_impl->Pointer, &pos.x, &pos.y);
		return pos;
	}
	std::string Window::GetTitle()const
	{
		const char* title = glfwGetWindowTitle(m_impl->Pointer);
		return std::string(title);
	}

	void Window::SetTitle(const std::string& newTitle)
	{
		glfwSetWindowTitle(m_impl->Pointer, newTitle.c_str());
	}
	void Window::SetSize(Vec2i newSize)
	{
		glfwSetWindowSize(m_impl->Pointer, newSize.x, newSize.y);
	}
	void Window::SetPosition(Vec2i newPosition)
	{
		glfwSetWindowPos(m_impl->Pointer, newPosition.x, newPosition.y);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}
	void Window::SwapBuffers()
	{
		//m_frameBuffer->Blit(0);
		glfwSwapBuffers(m_impl->Pointer);
	}

	void Window::SetFullscreen()
	{
		//MonitorInfo monitor = MonitorInfo::GetPrimaryMonitor();

		//glfwSetWindowMonitor(m_impl->Pointer, glfwGetPrimaryMonitor(), 100, 100, 300, 300, monitor.GetRefreshRate());
	}

	void Window::SetWindowed(Vec2i size, Vec2i pos)
	{
		glfwSetWindowMonitor(m_impl->Pointer, nullptr, pos.x, pos.y, size.x, size.y, GLFW_DONT_CARE);
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
		glfwSetWindowShouldClose(m_impl->Pointer, GLFW_TRUE);

		TWISTED_INFO("Window closed");
		return true;
	}

	HWND Window::GetWindowHandle()
	{
		return glfwGetWin32Window(m_impl->Pointer);
	}

	void* Window::GetContextAdress()
	{
		return glfwGetProcAddress;
	}
	void* Window::GetRawPointer()
	{
		return m_impl->Pointer;
	}

	URef<Window> Window::CreateNewWindow(const std::string& title, Vec2i size, Vec2i position)
	{
		GLFWwindow* windowPointer = nullptr;
		windowPointer = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, NULL);
		if (!windowPointer)
		{
			TWISTED_ERROR("CreateNewWindow() failure! GLFW window pointer creation failure");
			return nullptr;
		}

		URef<Window> window = std::make_unique<Window>(title, size, position);
		window->m_impl->Pointer = windowPointer;

		glfwMakeContextCurrent(windowPointer);
		glfwSetWindowUserPointer(windowPointer, window.get());
		glfwSetWindowPos(windowPointer, position.x, position.y);

		glfwSetWindowCloseCallback(windowPointer,
			[](GLFWwindow* pointer)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				window->CloseWindowEvent.Invoke();
			});
		glfwSetWindowSizeCallback(windowPointer,
			[](GLFWwindow* pointer, int newWidth, int newHeight)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				//window->m_frameBuffer->Resize(newWidth, newHeight);
				window->WindowResizeEvent.Invoke();
			});
		glfwSetKeyCallback(windowPointer,
			[](GLFWwindow* pointer, int key, int scancode, int action, int mods)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				//InputManager::GetInstance().KeyCallback(pointer, key, scancode, action, mods);
			});
		glfwSetMouseButtonCallback(windowPointer,
			[](GLFWwindow* pointer, int button, int action, int mods)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				//InputManager::GetInstance().MouseButtonCallback(pointer, button, action, mods);
			});

		TWISTED_INFO("Window Created");
		return window;
	}

}