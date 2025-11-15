#pragma once
#include "Twisted/Application/Application.h"

#include "EditorLayer.h"
#include "Twisted/Windowing/WindowLayer.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Twisted/Rendering/RenderLayer.h"

namespace Twisted::Editor
{
	class EditorLayer;

	class EditorApplication : public Application
	{
	public:
		EditorApplication() = default;

		void OnInit()override;
		void OnBeforeRun()override;
		void OnBeginFrame()override;
		void OnFrame()override;
		void OnEndFrame()override;
		void OnTerminate()override;

	private:
		void SetCallbacks();
		void CreateAppWindow();

		WindowLayer* m_windowLayer = nullptr;
		RenderLayer* m_renderLayer = nullptr;
		AssetsLayer* m_assetsLayer = nullptr;
		EditorLayer* m_editorLayer = nullptr;

		Window* m_window;
		

	};
}
