#pragma once

#include "AppCore.h"

namespace Twisted
{
	class Application;
	class TWISTED_API Service
	{
	public:
		Service(Application* app,int priority) :m_priority(priority), m_app(app) {}
		virtual ~Service() {}
		Service(const Service& other) = delete;
		Service(Service&& other) = delete;
		Service& operator=(const Service& other) = delete;
		Service& operator=(Service&& other) = delete;

		inline virtual void OnInit() {}
		inline virtual void OnBeforeRun() {}
		inline virtual void OnFrameBegin() {}
		inline virtual void OnFrame() {}
		inline virtual void OnFrameEnd() {}
		inline virtual void OnTerminate() {}

		int GetPriority()const { return m_priority; }
	protected:
		int m_priority;
		Application* m_app;
	};
}
