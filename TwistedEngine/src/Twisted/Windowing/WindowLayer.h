#pragma once
#include "AppCore.h"
#include "Twisted/Application/Application.h"
#include "Twisted/Application/Layer.h"
#include "Window.h"

namespace Twisted
{
	class TWISTED_API WindowLayer :public Layer
	{
	public:		
		WindowLayer(Application* app);
		~WindowLayer();

		Window* GetWindow() { return (m_window) ? m_window.get() : nullptr; }
		Window* CreateNewWindow(const std::string& title,const Vec2i& size, const Vec2i& position)
		{
			m_window = std::make_unique<Window>(title, size, position);
			return m_window.get();
		}

	private:
		URef<Window> m_window;
	};
}