#pragma once

#include "pch.h"
//#include "Twisted/Application.h"

namespace Twisted
{
	class Application;
}


namespace Twisted::Editor
{
	class EditorWindow
	{
	public:
		void Render(Application* app);
		virtual void RenderContent(Application* app) {};
		virtual std::string GetName() { return "New Window"; }
	};
}