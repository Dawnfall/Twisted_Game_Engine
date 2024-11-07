#pragma once

namespace Twisted
{
	class AppBase;

	class SystemBase
	{
	public:
		SystemBase() = default;
		virtual ~SystemBase() = default;
		virtual void Update(AppBase* app) = 0;
	};
}