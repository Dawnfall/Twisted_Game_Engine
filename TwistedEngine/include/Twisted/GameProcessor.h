#pragma once

#include "AppCore.h"
#include "Twisted/Application/Processor.h"
#include "Twisted/Gameing/GameService.h"
#include "Twisted/Application/TimeService.h"

namespace Twisted
{
	class TWISTED_API GameProcessor :public Processor
	{
	public:
		GameProcessor(Application* app) :Processor(app) {}
		void OnInit() override;
		void OnFrame()override;

	private:
		GameService* m_gameService;
		TimeService* m_timeService;
	};
}