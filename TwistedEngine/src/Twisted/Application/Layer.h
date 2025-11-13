#pragma once
#include "AppCore.h"

namespace Twisted
{
	class Application;
	class TWISTED_API Layer
	{
	public:
		Layer(Application* app) :
			m_app(app)
		{
		}

		Layer(const Layer& other) = delete;
		Layer(Layer&& other) = delete;
		Layer& operator=(const Layer& other) = delete;
		Layer& operator=(Layer&& other) = delete;

		virtual ~Layer() {}

		Application* GetApplication() { return m_app; }

	protected:
		Application* m_app;
	};
}

