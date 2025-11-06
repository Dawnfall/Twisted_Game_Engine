#pragma once

#include "Twisted/Application/Layer.h"
#include "Twisted/Rendering/FrameBuffer.h"

namespace Twisted
{
	class Window;
	class Application;
}

namespace Twisted::Editor
{
	class EditorLayer :public Layer
	{
	public:

		EditorLayer(Application* app) :
			Layer(app)
		{
		}

		void Init() override;
		void Render(Window* window);

	private:

		void RenderDockSpace();
		void RenderMenuBar(Window* window);
	};
}