#pragma once
#include "AppCore.h"
#include "Twisted/Application/Application.h"
#include "Twisted/Application/Layer.h"
#include "InputManager.h"
#include "Window.h"

namespace Twisted
{
	class TWISTED_API WindowLayer :public Layer
	{
	public:		
		WindowLayer(Application* app):Layer(app){ InitGLFW(); }
		~WindowLayer(){ TerminateGLFW(); }

		InputManager& GetInput() { return m_input; }

		Window* GetWindow() { return (m_window) ? m_window.get() : nullptr; }
		Window* CreateNewWindow(const std::string& title,const Vec2i& size, const Vec2i& position)
		{
			m_window = Window::CreateNewWindow(title, size, position);
			return m_window.get();
		}

	private:
		bool InitGLFW();
		void TerminateGLFW();

		URef<Window> m_window;
		InputManager m_input;
	};
}