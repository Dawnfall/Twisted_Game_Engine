#pragma once
#include "Application.h"

namespace Twisted
{
	class RuntimeBase
	{
	public:
		AppParams Params;

		virtual void OnInit(Application& app) = 0;
		virtual void OnBeforeRun(Application& app) = 0;
		virtual void OnRun(Application& app) = 0;
	};
}