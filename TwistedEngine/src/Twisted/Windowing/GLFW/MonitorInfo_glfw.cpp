#include "AppCore.h"
#ifdef GLFW_INCLUDE_NONE

#include "Twisted/Windowing/MonitorInfo.h"
#include <GLFW/glfw3.h>

namespace Twisted
{
	GLFWmonitor* getMonitor(int index)
	{
		int monitorCount;
		GLFWmonitor** allMonitorPointers = glfwGetMonitors(&monitorCount);

		if (monitorCount == 0 || index >= monitorCount)
			return nullptr;

		return allMonitorPointers[index];
	}

	Vec2i MonitorInfo::GetMonitorSize()
	{
		Vec2i size;
		glfwGetMonitorPhysicalSize(getMonitor(m_index), &size.x, &size.y);
		return size;
	}
	Vec2i MonitorInfo::GetMonitorPos()
	{
		Vec2i pos;
		glfwGetMonitorPos(getMonitor(m_index), &pos.x, &pos.y);
		return pos;
	}
	std::string MonitorInfo::GetWindowName()
	{
		return glfwGetMonitorName(getMonitor(m_index));
	}
	Vec2i MonitorInfo::GetResolution()
	{
		const GLFWvidmode* mode = glfwGetVideoMode(getMonitor(m_index));
		return Vec2i(mode->width, mode->height);
	}
	int MonitorInfo::GetRefreshRate()
	{
		const GLFWvidmode* mode = glfwGetVideoMode(getMonitor(m_index));
		return mode->refreshRate;
	}

	std::vector<MonitorInfo> MonitorInfo::GetAllMonitors()
	{
		int monitorCount;
		GLFWmonitor** allMonitorPointers = glfwGetMonitors(&monitorCount);

		std::vector<MonitorInfo> monitors;
		for (int i = 0; i < monitorCount; i++)
			monitors.emplace_back(i);
		return monitors;
	}

	MonitorInfo MonitorInfo::GetPrimaryMonitor()
	{
		return MonitorInfo(0);
	}
}

#endif