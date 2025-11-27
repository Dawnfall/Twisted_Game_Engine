#pragma once

#include "Twisted/Application/Service.h"

#include "UI/UIWindow.h"

namespace Twisted::Editor
{
	class UIService :public Service
	{
	public:
		UIService(Application* app) :Service(app) {}

		void Init(Window* window);
		void Render(Window* window);

		UIWindow& GetUIWindow() { return m_uiWindow; }

	private:
		UIWindow m_uiWindow;

	};
}