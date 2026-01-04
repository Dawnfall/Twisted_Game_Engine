#pragma once

#include "AppCore.h"

namespace Twisted
{
	class Application;
	class TWISTED_API Processor
	{
	public:
		Processor(Application* app) :m_app(app) {}
		Processor(const Processor& other) = delete;
		Processor(Processor&& other) = delete;
		Processor& operator=(const Processor& other) = delete;
		Processor& operator=(Processor&& other) = delete;

		inline virtual void OnInit() {}
		inline virtual void OnBeforeRun() {}
		inline virtual void OnFrameBegin() {}
		inline virtual void OnFrame() {}
		inline virtual void OnFrameEnd() {}
		inline virtual void OnTerminate() {}

	protected:
		Application* m_app;
	};
}