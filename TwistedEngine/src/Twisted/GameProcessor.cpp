#include "Twisted/GameProcessor.h"
#include "Twisted/Application/Application.h"
#include "Twisted/Application/TimeService.h"
#include "Twisted/Gameing/GameService.h"

namespace Twisted
{
	void GameProcessor::OnInit()
	{
		m_gameService = m_app->AddService<GameService>();
		m_timeService = m_app->AddService<TimeService>();

		m_gameService->CreateEmptyWorld();
	}
	void GameProcessor::OnFrame()
	{
		if (m_gameService->GameWorld)
		{
			m_gameService->GameWorld->UpdateFrame(m_timeService->GetDeltaTime());
			m_gameService->renderer.Render(*m_gameService->GameWorld);
		}
	}
}