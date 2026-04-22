#pragma once
#include "AppCore.h"
#include "Twisted/Application/Service.h"
#include "Twisted/Rendering/RenderConvert.h"
#include "Utils/BoundedQueue.h"
#include <thread>
#include <variant>
#include <queue>

namespace Twisted
{
	class Window;

	class TWISTED_API RenderService :public Service
	{
	public:

		void OnInit() override;
		void OnTerminate() override;

		RenderService(Application* app, int priority);

		void SubmitContext(RenderContext context);

		void Render();

	private:

		std::queue<RenderContext> m_queue;
		Window* m_window;
	};
}
