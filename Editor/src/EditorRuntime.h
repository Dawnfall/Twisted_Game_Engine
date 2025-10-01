#pragma once
#include "Twisted/Application/RuntimeBase.h"

#include <string>

namespace Twisted
{
	class Application;
	class WindowLayer;
	class RenderLayer;
	class AssetsLayer;
	class Window;
}

namespace Twisted::Editor
{
	class EditorLayer;
	class ProjectLoader;

	class EditorRuntime :public RuntimeBase
	{
	private:
		std::string layoutFilePath = "";

		Window* m_window = nullptr;

		WindowLayer* m_windowLayer = nullptr;
		RenderLayer* m_renderLayer = nullptr;
		AssetsLayer* m_assetsLayer = nullptr;
		EditorLayer* m_editorLayer = nullptr;
		ProjectLoader* m_projectLoader = nullptr;

	public:

		void OnCreate()override;
		void OnBeforeRun()override;
		void OnFrame()override;
		void OnTerminate()override;

	private:
		void LoadBuiltIn();

		void StartWindow();
		void LoadResources();

		void CreateObjects();

		void RegisterLayers();
		void RegisterPanels();
		void RegisterImporters();
		void RegisterPainters();
	};
}