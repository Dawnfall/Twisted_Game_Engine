#pragma once

#include "AppCore.h"

namespace Twisted
{
	class Application;
	class TWISTED_API Service
	{
	public:
		Service(Application* app) :m_app(app) {}
		virtual ~Service() {}
		Service(const Service& other) = delete;
		Service(Service&& other) = delete;
		Service& operator=(const Service& other) = delete;
		Service& operator=(Service&& other) = delete;

		virtual void OnInit() {};
		virtual void OnTerminate() {};

	protected:
		Application* m_app;
	};
}