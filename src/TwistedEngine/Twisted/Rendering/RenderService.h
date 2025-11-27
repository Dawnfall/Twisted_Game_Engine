#pragma once
#include "AppCore.h"
#include "Twisted/Application/Service.h"

namespace Twisted
{
	class TWISTED_API RenderService :public Service
	{
	public:
		RenderService(Application* app):Service(app){}
	private:
	};
}