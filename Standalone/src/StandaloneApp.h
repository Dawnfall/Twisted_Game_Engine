#pragma once
#include "Twisted/AppBase.h"
#include "Twisted/Rendering/RenderingAPI.h"

namespace Twisted::Standalone
{
	class StandaloneApp :public Twisted::AppBase
	{
		virtual void Run(const Twisted::AppParams& params, Twisted::RuntimeBase* runtime) override
		{
			if (IsRunning())
			{
				TWISTED_WARN("Cannot start application; Application already running!");
				return;
			}

			m_runtime = runtime;
			if (!Init(params))
				return;

			CreateNewWindow(params);

			m_isRunning = true;
			while (IsRunning())
			{
				WindowAPI::PollEvents();
				RenderAPI::ClearWindow(Colors::blue);

				//m_world->UpdateFrame(this,m_time);

				WindowAPI::SwapBuffers(GetWindow());

				//if (m_runtime)
				//	m_runtime->OnRun(this);
			}
		}
	};
}