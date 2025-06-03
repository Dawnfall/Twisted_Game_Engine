#pragma once
#include "AppCore.h"

namespace Twisted
{
	class Application;
	class TWISTED_API Layer
	{
	public:
		Layer(Application* app):
			m_app(app)
		{ }

		virtual void OnInit() {}
		virtual void OnFrameBegin() {}
		virtual void OnFrame() {}
		virtual void OnFrameEnd() {}
		virtual void OnTerminate() {}
		virtual void OnBeforeRun() {}
	
	protected:
		Application* m_app;
	};
}