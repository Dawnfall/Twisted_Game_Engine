#pragma once

#include "AppCore.h"

namespace Twisted
{
	class AppBase;
	class TWISTED_API RuntimeBase
	{
	public:
		virtual void OnInit(AppBase* app) = 0;
		virtual void OnBeforeRun(AppBase* app) = 0;
		virtual void OnRun(AppBase* app) = 0;
	};
}