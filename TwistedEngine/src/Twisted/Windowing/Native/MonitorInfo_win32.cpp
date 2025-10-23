#include "AppCore.h"
#ifdef NATIVE_USE

#include "Twisted/Windowing/MonitorInfo.h"
#include <Windows.h>

namespace Twisted
{
    struct MonitorEnumData
    {
        std::vector<HMONITOR> handles;
    };

    static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData)
    {
        auto* data = reinterpret_cast<MonitorEnumData*>(dwData);
        data->handles.push_back(hMonitor);
        return TRUE;
    }

    static HMONITOR GetMonitorHandle(int index)
    {
        MonitorEnumData data;
        EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&data));
        if (index < 0 || index >= static_cast<int>(data.handles.size()))
            return nullptr;
        return data.handles[index];
    }

	Vec2i MonitorInfo::GetMonitorSize()
	{
        // There’s no exact “physical size” in mm in Win32 without DPI APIs, 
                // so you can approximate using resolution / DPI.
        Vec2i res = GetResolution();
        HMONITOR hMon = GetMonitorHandle(m_index);
        if (!hMon) return { 0, 0 };

        UINT dpiX = 96, dpiY = 96;
        // Windows 8.1+ only, guard it if needed
        HMODULE shcore = LoadLibraryA("Shcore.dll");
        if (shcore)
        {
            typedef HRESULT(WINAPI* GetDpiForMonitorProc)(HMONITOR, int, UINT*, UINT*);
            auto getDpi = (GetDpiForMonitorProc)GetProcAddress(shcore, "GetDpiForMonitor");
            if (getDpi)
                getDpi(hMon, 0, &dpiX, &dpiY);
            FreeLibrary(shcore);
        }

        // Convert pixels ? mm roughly (25.4 mm per inch)
        return { static_cast<int>(res.x * 25.4f / dpiX), static_cast<int>(res.y * 25.4f / dpiY) };
	}
	Vec2i MonitorInfo::GetMonitorPos()
	{
        MONITORINFO info = { sizeof(MONITORINFO) };
        if (GetMonitorInfo(GetMonitorHandle(m_index), &info))
            return { info.rcMonitor.left, info.rcMonitor.top };
        return { 0, 0 };
	}
	std::string MonitorInfo::GetWindowName()
	{
        MONITORINFOEX info = { sizeof(MONITORINFOEX) };
        if (GetMonitorInfo(GetMonitorHandle(m_index), &info))
            return info.szDevice; // Example: "\\.\DISPLAY1"
        return "Unknown";
	}

    Vec2i MonitorInfo::GetResolution()
    {
        MONITORINFOEX info = { sizeof(MONITORINFOEX) };
        if (!GetMonitorInfo(GetMonitorHandle(m_index), &info))
            return { 0, 0 };

        DEVMODE devMode = {};
        devMode.dmSize = sizeof(DEVMODE);
        if (!EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &devMode))
            return { 0, 0 };

        return { static_cast<int>(devMode.dmPelsWidth), static_cast<int>(devMode.dmPelsHeight) };
    }

    int MonitorInfo::GetRefreshRate()
    {
        MONITORINFOEX info = { sizeof(MONITORINFOEX) };
        if (!GetMonitorInfo(GetMonitorHandle(m_index), &info))
            return 0;

        DEVMODE devMode = {};
        devMode.dmSize = sizeof(DEVMODE);
        if (!EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &devMode))
            return 0;

        return static_cast<int>(devMode.dmDisplayFrequency);
    }

    std::vector<MonitorInfo> MonitorInfo::GetAllMonitors()
    {
        MonitorEnumData data;
        EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&data));

        std::vector<MonitorInfo> monitors;
        for (int i = 0; i < static_cast<int>(data.handles.size()); i++)
            monitors.emplace_back(i);
        return monitors;
    }

    MonitorInfo MonitorInfo::GetPrimaryMonitor()
    {
        // The primary is always index 0 according to EnumDisplayMonitors order.
        return MonitorInfo(0);
    }
}
#endif