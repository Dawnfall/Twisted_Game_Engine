#include "Twisted/Windowing/WindowsService.h"
#include "Twisted/Windowing/Input.h"

#ifdef _WIN32
#include "Twisted/Windowing/WIN32/WindowsUtils_Win32.h"
#else
#include <GLFW/glfw3.h>
#endif

namespace Twisted
{
	void WindowsService::PollEvents()
	{
		Input::GetInstance().BeginFrame();

#ifdef _WIN32
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			PollMsgEvent.Invoke(&msg);
			DispatchMessage(&msg);
		}
#else
		glfwPollEvents();
#endif
	}

	void WindowsService::DispatchEvent(const KeyEvent& e)
	{
		Input::GetInstance().UpdateKey(e.key, e.state);
	}
	void WindowsService::DispatchEvent(const MouseButtonEvent& e)
	{
		Input::GetInstance().UpdateMouseButton(e.button, e.state);
	}
	void WindowsService::DispatchEvent(const MouseMoveEvent& e)
	{
		Input::GetInstance().UpdateMousePosition(e.position.x, e.position.y);
	}
	void WindowsService::DispatchEvent(const MouseWheelEvent& e)
	{
		Input::GetInstance().UpdateMouseWheel(e.delta);
	}
	void WindowsService::DispatchEvent(const WindowCloseEvent&)
	{
		if (m_window)
			m_window->CloseWindowEvent.Invoke();
	}
	void WindowsService::DispatchEvent(const WindowResizeEvent& e)
	{
		if (m_window)
			m_window->WindowResizeEvent.Invoke(e.size);
	}
	void WindowsService::DispatchEvent(const WindowFocusEvent& e)
	{
		if (!e.gained)
			Input::GetInstance().ClearAllInput();
	}

	Window* WindowsService::CreateNewWindow(const std::string& title, const Vec2i& size, const Vec2i& position)
	{
		m_window = std::make_unique<Window>(this, title, size, position);
#ifdef _WIN32
		SetWindowLongPtr((HWND)m_window->GetRawPointer(), GWLP_USERDATA, (LONG_PTR)m_window.get());
#endif
		return m_window.get();
	}
	void WindowsService::DestroyWindow(Window* window)
	{
		if (window && m_window && window == m_window.get())
			m_window = nullptr;
	}

}
