#pragma once
#include "AppCore.h"
#include "Twisted/Application/Application.h"
#include "Twisted/Application/Service.h"
#include "Window.h"
#include "Twisted/Windowing/WindowCoreAPI.h"
#include "Twisted/AssetsLayer/AssetsService.h"

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

	private:

		URef<Window> m_window;
	};
}

