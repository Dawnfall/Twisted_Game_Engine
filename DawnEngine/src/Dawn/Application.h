#pragma once

#include <memory>
#include "Window.h"

namespace Dawn
{
	class Application
	{
	public:
		Application();

		void Init();

		void Run();

	private:
		std::unique_ptr<Window> m_window;

	};
}