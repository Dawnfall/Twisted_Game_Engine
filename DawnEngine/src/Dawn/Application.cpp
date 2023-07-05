#include "Application.h"

#include "../Logger.h"
#include "Window.h"

namespace Dawn
{
	Application::Application()
	{
	}

	Application::~Application()
	{
		Window::Terminate();
	}

	void Application::Init()
	{
		Logger::Init();

		bool isWindowInit = Window::Init();
		//DAWN_DEBUG_ASSERT(isWindowInit);

		m_window = std::unique_ptr<Window>(Window::Create());


		//DAWN_DEBUG_ASSERT(m_window != nullptr);
		DAWN_INFO("Window Created");
		//DAWN_INFO("Application Init success!");
	}

	void Application::Run()
	{
		while (!m_window->ShouldClose())
		{
			m_window->OnUpdate();
		}
	}


}