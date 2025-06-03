#pragma once

#include "AppCore.h"
#include <chrono>
#include <array>

namespace Twisted
{
	class TWISTED_API TimeManager
	{
	public:
		void Start()
		{
			m_startAppTime = m_updateLastTwoTimes[0] = m_updateLastTwoTimes[1] = std::chrono::high_resolution_clock::now();
		}

		void UpdateNewFrame()
		{
			m_updateLastTwoTimes[0] = m_updateLastTwoTimes[1];
			m_updateLastTwoTimes[1] = std::chrono::high_resolution_clock::now();
			m_frameCount++;
		}

		unsigned long long GetFrameCount() const { return m_frameCount; }
		float GetTimeSinceAppStart()const { return std::chrono::duration<float>(m_updateLastTwoTimes[1] - m_startAppTime).count(); }
		float GetDeltaFrameTime()const { return std::chrono::duration<float>(m_updateLastTwoTimes[1] - m_updateLastTwoTimes[0]).count(); }

	private:
		std::chrono::steady_clock::time_point m_startAppTime;
		std::array<std::chrono::steady_clock::time_point, 2> m_updateLastTwoTimes;

		unsigned int m_frameCount = 0;
		float m_targetedFrameDeltaTime = 0.0f;
	};
}

//TODO..... for the profiler

//float deltaRelativeTime = (m_updateLastTwoTimes[1] - m_frameRateLastRefTime).count();
//m_relativeFrameCount++;
//
//if (deltaRelativeTime >= FRAMERATE_CAPTURE_TIME)
//{
//	m_frameRate = m_relativeFrameCount / deltaRelativeTime;
//	m_frameRateLastRefTime = m_updateLastTwoTimes[1];
//
//	m_relativeFrameCount = 0;
//}
//const float FRAMERATE_CAPTURE_TIME = 1.0f;
