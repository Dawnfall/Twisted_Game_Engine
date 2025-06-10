#pragma once

namespace Twisted
{
	class Application;
	class RuntimeBase
	{
	public:
		virtual void OnCreate() {}
		virtual void OnInit() {}
		virtual void OnBeforeRun() {}
		virtual void OnFrame() {}
		virtual void OnTerminate() {}

		Application* App;
	};

}