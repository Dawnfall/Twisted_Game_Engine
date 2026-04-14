#ifndef _WIN32

#include "Twisted/Windowing/Window.h"
#include "Twisted/Windowing/WindowsService.h"
#include "Debug/Logger.h"

namespace Twisted
{
    // Minimal stub backend — Linux windowing not yet implemented
    struct WindowBackend {};

    Window::Window(WindowsService* service, const std::string& /*title*/, Vec2i /*size*/, Vec2i /*position*/) :
        m_backend(std::make_unique<WindowBackend>()),
        m_windowsService(service)
    {
        TWISTED_WARN("Window creation is not implemented on Linux");
    }

    Window::~Window() = default;
    Window::Window(Window&& other) = default;
    Window& Window::operator=(Window&& other) = default;

    Vec2i Window::GetSize() const { return {}; }
    Vec2i Window::GetClientSize() const { return {}; }
    Vec2i Window::GetPosition() const { return {}; }
    std::string Window::GetTitle() const { return {}; }
    bool Window::IsWindowMaximized() const { return false; }
    Window::RestoreBounds Window::GetRestoreBounds() const { return {}; }

    void Window::SetTitle(const std::string&) {}
    void Window::SetSize(Vec2i) {}
    void Window::SetPosition(Vec2i) {}
    void Window::Minimize() {}
    void Window::Maximize() {}
    void Window::Restore() {}
    void Window::SetFullScreen(bool) {}
    void Window::SetWindowed(Vec2i, Vec2i) {}
    bool Window::CloseWindow() { return false; }
    void Window::Clear(const Color&) {}
    void Window::SetVSync(int) {}
    void Window::SwapBuffers() {}
    void* Window::GetRawPointer() { return nullptr; }
}

#endif // !_WIN32
