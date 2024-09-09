#pragma once

namespace Twisted
{
	class TimeManager
	{
	public:
		void Init()
		{
			m_startAppTime = std::chrono::high_resolution_clock::now();
		}

		void UpdateClocks()
		{
			m_updateLastTwoTimes[0] = m_updateLastTwoTimes[1];
			m_updateLastTwoTimes[1] = std::chrono::high_resolution_clock::now();

			m_deltaSinceAppStart = (float)(m_updateLastTwoTimes[1] - m_startAppTime).count();
			m_deltaSinceLastFrame += (m_updateLastTwoTimes[1] - m_updateLastTwoTimes[0]).count();
		}

		void IncreaseFrameCount() { m_frameCount++; }
		bool IsNextFrame()const { return m_deltaSinceLastFrame >= m_targetedFrameDeltaTime; }
		void ResetFrameTime() { m_deltaSinceLastFrame = 0.0f; }

		unsigned int GetFrameCount() const { return m_frameCount; }
		float GetTimeSinceAppStart()const { return m_deltaSinceAppStart; }
		float GetDeltaFrameTime()const { return m_deltaSinceLastFrame; }

	private:
		std::chrono::steady_clock::time_point m_startAppTime;
		std::chrono::steady_clock::time_point m_updateLastTwoTimes[2];

		float m_deltaSinceAppStart;
		float m_deltaSinceLastFrame;

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
