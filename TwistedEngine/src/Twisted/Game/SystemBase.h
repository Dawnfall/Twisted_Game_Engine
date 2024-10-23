#pragma once

namespace Twisted
{
	class Application;

	class SystemBase
	{
	public:
		SystemBase() = default;
		virtual ~SystemBase() = default;
		virtual void Update(Application* app) = 0;
	};
}