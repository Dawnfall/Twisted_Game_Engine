#include "UIService.h"

#include <Twisted/Windowing/Window.h>
#include "EditorApp/EditorRegistry.h"
#include "UI/ImguiExtensions.h"

namespace Twisted::Editor
{
	void UIService::Init(Window* window)
	{
		Im::Init(window);
		for (auto& panel : EditorRegistry::GetInstance().m_panels)
			panel->Init();
	}

	void UIService::Render(Window* window)
	{
		m_uiWindow.Render(window);
	}
}