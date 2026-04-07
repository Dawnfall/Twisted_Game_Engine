#pragma once
#include "AppCore.h"
#include "Twisted/Application/Processor.h"

#include "EditorApp/EditorService.h"
#include "Twisted/Windowing/WindowsService.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include "Twisted/Gameing/GameService.h"
#include "Twisted/Application/TimeService.h"
#include "Twisted/Rendering/RenderService.h"

#include "Twisted/Rendering/Data/RenderContext.h"
#include "Twisted/Rendering/Data/ClearParams.h"

#include <vector>

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

		EditorService* m_editorEventsService = nullptr;
		WindowsService* m_windowsService = nullptr;
		AssetsService* m_assetsService = nullptr;
		TimeService* m_timeService = nullptr;
		GameService* m_gameService = nullptr;
		RenderService* m_renderService = nullptr;

		World* m_gameWorld = nullptr;
		World* m_editorWorld = nullptr;
		Entity m_editorCameraEnt = Entity::Invalid();

		ClearParams m_clearParams;
		unsigned int m_clearBits = 0;
	};
}