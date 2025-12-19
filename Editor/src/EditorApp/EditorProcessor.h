#pragma once
#include "AppCore.h"
#include "Twisted/Application/Processor.h"

#include "EditorApp/EditorWorldService.h"
#include "Twisted/Windowing/WindowsService.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include "UI/UIService.h"

namespace Twisted::Editor
{
	class EditorProcessor :public Processor
	{
	public:
		EditorProcessor(Application* app) :Processor(app) {}

		void OnInit()override;
		void OnBeforeRun()override;
		void OnFrameBegin()override;
		void OnFrame()override;
		void OnFrameEnd()override;
		void OnTerminate()override;

	private:
		void CreateAppWindow();

		UIService* m_uiService = nullptr;
		EditorWorldService* m_editorService = nullptr;
		WindowsService* m_windowsService = nullptr;
		AssetsService* m_assetsService = nullptr;
	};
}