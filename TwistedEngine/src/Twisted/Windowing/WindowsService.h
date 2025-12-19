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
			Windowing::Init();
		}
		~WindowsService() 
		{
			Windowing::Terminate();
		}

		Window* GetWindow() { return (m_window) ? m_window.get() : nullptr; }
		Window* CreateNewWindow(const std::string& title, const Vec2i& size, const Vec2i& position);
		void DestroyWindow(Window* window);
	private:

		URef<Window> m_window;
	};
}

