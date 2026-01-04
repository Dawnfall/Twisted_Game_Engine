#include "Twisted/Windowing/WindowsService.h"

namespace Twisted
{
	Window* WindowsService::CreateNewWindow(const std::string& title, const Vec2i& size, const Vec2i& position)
	{
		m_window = std::make_unique<Window>(title, size, position);
		return m_window.get();
	}
	void WindowsService::DestroyWindow(Window* window)
	{
		if (window && m_window && window == m_window.get())
			m_window = nullptr;
	}

}