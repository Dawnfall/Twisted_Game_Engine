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
		void RenderTitleBar(Window* window);
		void RenderMenuBar(Window* window);

		void SelectProject(std::filesystem::path& path);
	};
}
