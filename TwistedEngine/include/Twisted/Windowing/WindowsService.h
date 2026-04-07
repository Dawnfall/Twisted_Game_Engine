#pragma once
#include "AppCore.h"
#include "Twisted/Application/Application.h"
#include "Twisted/Application/Service.h"
#include "Window.h"
#include "Twisted/Windowing/WindowCoreAPI.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include "Twisted/Windowing/WindowEvents.h"

namespace Twisted
{
	class TWISTED_API WindowsService :public Service
	{
	public:
		WindowsService(Application* app) :Service(app)
		{

		}
		~WindowsService()
		{

		}

		void PollEvents();

		Window* GetWindow() { return (m_window) ? m_window.get() : nullptr; }
		Window* CreateNewWindow(const std::string& title, const Vec2i& size, const Vec2i& position);
		void DestroyWindow(Window* window);

		inline static Event<void*> PollMsgEvent;
		
		void DispatchEvent(const KeyEvent& e);
		void DispatchEvent(const MouseButtonEvent& e);
		void DispatchEvent(const MouseMoveEvent& e);
		void DispatchEvent(const MouseWheelEvent& e);
		void DispatchEvent(const WindowCloseEvent& e);
		void DispatchEvent(const WindowResizeEvent& e);
		void DispatchEvent(const WindowFocusEvent& e);

	private:

		URef<Window> m_window;
	};
}

