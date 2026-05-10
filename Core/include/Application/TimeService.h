#pragma once
#include "Service.h"
#include <chrono>

using Clock = std::chrono::steady_clock;
using Time = std::chrono::steady_clock::time_point;
namespace Twisted
{
	class TimeService :public Service
	{
	public:
		TimeService(Application* app,int priority) :Service(app, priority)
		{
			s_instance = this;
		}
		inline static TimeService* GetInstance() { return s_instance; }

		float GetDeltaTime()const { return std::chrono::duration<float>(m_currentTime - m_lastFrameTime).count()* m_timeScale; }
		float GetTimeSinceStart()const { return std::chrono::duration<float>(m_currentTime - m_appStartTime).count(); }
		void SetTimeScale(float scale) { m_timeScale = scale; }
		float GetTimeScale()const { return m_timeScale; }
		uint64_t GetCurrentFrame()const { return m_frameCount; }

	private:
		void OnFrameBegin() override
		{
			Time currTime = m_clock.now();

			m_frameCount += 1;
			if (m_frameCount == 1)
			{
				m_appStartTime = currTime;
				m_lastFrameTime = currTime;
				m_currentTime = currTime;
			}
			else
			{
				m_lastFrameTime = m_currentTime;
				m_currentTime = currTime;
			}
		}

		Clock m_clock;
		Time m_appStartTime;
		Time m_lastFrameTime;
		Time m_currentTime;
		float m_timeScale = 1.0f;

		uint64_t m_frameCount = 0;

		inline static TimeService* s_instance = nullptr;
		friend class Application;
	};


}