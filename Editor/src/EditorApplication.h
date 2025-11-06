#pragma once
#include "Twisted/Application/Application.h"

inline static std::string layoutFilePath = "";

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

	class EditorApplication : public Application
	{
	public:
		EditorApplication() = default;

	private:

		void OnCreate() override;
		void OnBeforeRun() override;
		void OnFrame() override;
		void OnTerminate()override;

	private:

		void LoadBuiltIn();
		void StartWindow();
		void LoadResources();
		void RegisterLayers();
		void Create();

	private:
		Window* m_window = nullptr;

		WindowLayer* m_windowLayer = nullptr;
		RenderLayer* m_renderLayer = nullptr;
		AssetsLayer* m_assetsLayer = nullptr;
		EditorLayer* m_editorLayer = nullptr;
		ProjectLoader* m_projectLoader = nullptr;
	};
}