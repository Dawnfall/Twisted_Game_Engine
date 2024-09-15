#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Twisted/Application.h"

class Twisted::Application;

namespace Twisted::Editor
{
	class Editor
	{
	public:
		bool m_showDemo = true;
		void Init(Application& app);
		void Update(Application& app);
		void Terminate(Application& app);

	private:
		void InitImgui(Application& app);
		void TerminateImgui(Application& app);
	};
}