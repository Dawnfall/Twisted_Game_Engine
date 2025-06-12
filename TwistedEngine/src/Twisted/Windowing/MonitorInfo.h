#pragma once
#include "AppCore.h"
#include "Utils/GlmUtils.h"

#include <string>
#include <vector>

namespace Twisted
{
	class TWISTED_API MonitorInfo
	{
	public:
		explicit MonitorInfo(int index) :m_index(index) {}

		Vec2i GetMonitorSize();
		Vec2i GetMonitorPos();
		std::string GetWindowName();
		Vec2i GetResolution();
		int GetRefreshRate();

		static std::vector<MonitorInfo> GetAllMonitors();
		static MonitorInfo GetPrimaryMonitor();

	private:
		//explicit MonitorInfo(int index) :m_index(index) {}
		int m_index = -1;
	};
}
