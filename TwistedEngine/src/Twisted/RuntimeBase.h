#pragma once
#include "AppParams.h"

namespace Twisted
{
	class Application;

	class RuntimeBase
	{
	public:
		AppParams Params;

		virtual void OnInit(Twisted::Application* app) = 0;
		virtual void OnBeforeRun(Twisted::Application* app) = 0;
		virtual void OnRun(Twisted::Application* app) = 0;
	};
}