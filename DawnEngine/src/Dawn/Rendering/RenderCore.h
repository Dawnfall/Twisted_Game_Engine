#pragma once

#include <memory>

#include "Dawn/Window.h"
#include "Debug/Logger.h"
#include "Dawn/Application.h"

namespace Dawn
{
	class Application;

	class RenderCore
	{
	public:
		void Init();
		void Update(Application* app);


		void CloseWindow()
		{
			if (m_window)
				m_window->Close();
		}

		const std::unique_ptr<Window>& GetWindow() { return m_window; }
	private:
		std::unique_ptr<Window> m_window;
	};
}
