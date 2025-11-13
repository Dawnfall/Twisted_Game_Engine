#pragma once
#include "Twisted/Windowing/Window.h"

namespace Twisted::Editor
{
	class UIWindow
	{
	public:
		void Render(Window* window);

	private:
		void RenderDockSpace();
		void RenderMenuBar(Window* window);
	};
}
