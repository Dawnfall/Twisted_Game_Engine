#pragma once

#include "AppCore.h"

namespace Twisted
{
	class AppBase;
	class TWISTED_API SystemBase
	{
	public:
		virtual ~SystemBase() = default;
		virtual void Update(AppBase* app) = 0;
	};
}