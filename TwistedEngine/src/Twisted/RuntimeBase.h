#pragma once

#include "AppCore.h"

namespace Twisted
{
	class AppBase;

	class TWISTED_API RuntimeBase
	{
	public:
		virtual void OnInit(Twisted::AppBase* app) = 0;
		virtual void OnBeforeRun(Twisted::AppBase* app) = 0;
		virtual void OnRun(Twisted::AppBase* app) = 0;
	};
}