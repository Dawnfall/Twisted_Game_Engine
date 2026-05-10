#include "GLFW/Window_GLFW.h"
#include "Window.h"
#include "WindowsService.h"
#include "WindowEvents.h"
#include "Debug/Logger.h"
#include <glad/glad.h>
#include <filesystem>

namespace Twisted
{

// ---------------------------------------------------------------------------
// Key / button translation
// ---------------------------------------------------------------------------

static Key glfwToTwistedKey(int k) noexcept
{
    switch (k)
    {
    case GLFW_KEY_SPACE:         return Key::Space;
    case GLFW_KEY_APOSTROPHE:    return Key::Apostrophe;
    case GLFW_KEY_COMMA:         return Key::Comma;
    case GLFW_KEY_MINUS:         return Key::Minus;
    case GLFW_KEY_PERIOD:        return Key::Period;
    case GLFW_KEY_SLASH:         return Key::Slash;
    case GLFW_KEY_0:             return Key::D0;
    case GLFW_KEY_1:             return Key::D1;
    case GLFW_KEY_2:             return Key::D2;
    case GLFW_KEY_3:             return Key::D3;
    case GLFW_KEY_4:             return Key::D4;
    case GLFW_KEY_5:             return Key::D5;
    case GLFW_KEY_6:             return Key::D6;
    case GLFW_KEY_7:             return Key::D7;
    case GLFW_KEY_8:             return Key::D8;
    case GLFW_KEY_9:             return Key::D9;
    case GLFW_KEY_SEMICOLON:     return Key::Semicolon;
    case GLFW_KEY_EQUAL:         return Key::Equal;
    case GLFW_KEY_A:             return Key::A;
    case GLFW_KEY_B:             return Key::B;
    case GLFW_KEY_C:             return Key::C;
    case GLFW_KEY_D:             return Key::D;
    case GLFW_KEY_E:             return Key::E;
    case GLFW_KEY_F:             return Key::F;
    case GLFW_KEY_G:             return Key::G;
    case GLFW_KEY_H:             return Key::H;
    case GLFW_KEY_I:             return Key::I;
    case GLFW_KEY_J:             return Key::J;
    case GLFW_KEY_K:             return Key::K;
    case GLFW_KEY_L:             return Key::L;
    case GLFW_KEY_M:             return Key::M;
    case GLFW_KEY_N:             return Key::N;
    case GLFW_KEY_O:             return Key::O;
    case GLFW_KEY_P:             return Key::P;
    case GLFW_KEY_Q:             return Key::Q;
    case GLFW_KEY_R:             return Key::R;
    case GLFW_KEY_S:             return Key::S;
    case GLFW_KEY_T:             return Key::T;
    case GLFW_KEY_U:             return Key::U;
    case GLFW_KEY_V:             return Key::V;
    case GLFW_KEY_W:             return Key::W;
    case GLFW_KEY_X:             return Key::X;
    case GLFW_KEY_Y:             return Key::Y;
    case GLFW_KEY_Z:             return Key::Z;
    case GLFW_KEY_LEFT_BRACKET:  return Key::LeftBracket;
    case GLFW_KEY_BACKSLASH:     return Key::Backslash;
    case GLFW_KEY_RIGHT_BRACKET: return Key::RightBracket;
    case GLFW_KEY_GRAVE_ACCENT:  return Key::GraveAccent;
    case GLFW_KEY_WORLD_1:       return Key::World1;
    case GLFW_KEY_WORLD_2:       return Key::World2;
    case GLFW_KEY_ESCAPE:        return Key::Escape;
    case GLFW_KEY_ENTER:         return Key::Enter;
    case GLFW_KEY_TAB:           return Key::Tab;
    case GLFW_KEY_BACKSPACE:     return Key::Backspace;
    case GLFW_KEY_INSERT:        return Key::Insert;
    case GLFW_KEY_DELETE:        return Key::Delete;
    case GLFW_KEY_RIGHT:         return Key::Right;
    case GLFW_KEY_LEFT:          return Key::Left;
    case GLFW_KEY_DOWN:          return Key::Down;
    case GLFW_KEY_UP:            return Key::Up;
    case GLFW_KEY_PAGE_UP:       return Key::PageUp;
    case GLFW_KEY_PAGE_DOWN:     return Key::PageDown;
    case GLFW_KEY_HOME:          return Key::Home;
    case GLFW_KEY_END:           return Key::End;
    case GLFW_KEY_CAPS_LOCK:     return Key::CapsLock;
    case GLFW_KEY_SCROLL_LOCK:   return Key::ScrollLock;
    case GLFW_KEY_NUM_LOCK:      return Key::NumLock;
    case GLFW_KEY_PRINT_SCREEN:  return Key::PrintScreen;
    case GLFW_KEY_PAUSE:         return Key::Pause;
    case GLFW_KEY_F1:            return Key::F1;
    case GLFW_KEY_F2:            return Key::F2;
    case GLFW_KEY_F3:            return Key::F3;
    case GLFW_KEY_F4:            return Key::F4;
    case GLFW_KEY_F5:            return Key::F5;
    case GLFW_KEY_F6:            return Key::F6;
    case GLFW_KEY_F7:            return Key::F7;
    case GLFW_KEY_F8:            return Key::F8;
    case GLFW_KEY_F9:            return Key::F9;
    case GLFW_KEY_F10:           return Key::F10;
    case GLFW_KEY_F11:           return Key::F11;
    case GLFW_KEY_F12:           return Key::F12;
    case GLFW_KEY_F13:           return Key::F13;
    case GLFW_KEY_F14:           return Key::F14;
    case GLFW_KEY_F15:           return Key::F15;
    case GLFW_KEY_F16:           return Key::F16;
    case GLFW_KEY_F17:           return Key::F17;
    case GLFW_KEY_F18:           return Key::F18;
    case GLFW_KEY_F19:           return Key::F19;
    case GLFW_KEY_F20:           return Key::F20;
    case GLFW_KEY_F21:           return Key::F21;
    case GLFW_KEY_F22:           return Key::F22;
    case GLFW_KEY_F23:           return Key::F23;
    case GLFW_KEY_F24:           return Key::F24;
    case GLFW_KEY_F25:           return Key::F25;
    case GLFW_KEY_KP_0:          return Key::KP_0;
    case GLFW_KEY_KP_1:          return Key::KP_1;
    case GLFW_KEY_KP_2:          return Key::KP_2;
    case GLFW_KEY_KP_3:          return Key::KP_3;
    case GLFW_KEY_KP_4:          return Key::KP_4;
    case GLFW_KEY_KP_5:          return Key::KP_5;
    case GLFW_KEY_KP_6:          return Key::KP_6;
    case GLFW_KEY_KP_7:          return Key::KP_7;
    case GLFW_KEY_KP_8:          return Key::KP_8;
    case GLFW_KEY_KP_9:          return Key::KP_9;
    case GLFW_KEY_KP_DECIMAL:    return Key::KP_Decimal;
    case GLFW_KEY_KP_DIVIDE:     return Key::KP_Divide;
    case GLFW_KEY_KP_MULTIPLY:   return Key::KP_Multiply;
    case GLFW_KEY_KP_SUBTRACT:   return Key::KP_Subtract;
    case GLFW_KEY_KP_ADD:        return Key::KP_Add;
    case GLFW_KEY_KP_ENTER:      return Key::KP_Enter;
    case GLFW_KEY_KP_EQUAL:      return Key::KP_Equal;
    case GLFW_KEY_LEFT_SHIFT:    return Key::LeftShift;
    case GLFW_KEY_LEFT_CONTROL:  return Key::LeftControl;
    case GLFW_KEY_LEFT_ALT:      return Key::LeftAlt;
    case GLFW_KEY_LEFT_SUPER:    return Key::LeftSuper;
    case GLFW_KEY_RIGHT_SHIFT:   return Key::RightShift;
    case GLFW_KEY_RIGHT_CONTROL: return Key::RightControl;
    case GLFW_KEY_RIGHT_ALT:     return Key::RightAlt;
    case GLFW_KEY_RIGHT_SUPER:   return Key::RightSuper;
    case GLFW_KEY_MENU:          return Key::Menu;
    default:                     return Key::Invalid;
    }
}

static MouseButton glfwToTwistedButton(int b) noexcept
{
    switch (b)
    {
    case GLFW_MOUSE_BUTTON_LEFT:   return MouseButton::Left;
    case GLFW_MOUSE_BUTTON_RIGHT:  return MouseButton::Right;
    case GLFW_MOUSE_BUTTON_MIDDLE: return MouseButton::Middle;
    case GLFW_MOUSE_BUTTON_4:      return MouseButton::Button4;
    case GLFW_MOUSE_BUTTON_5:      return MouseButton::Button5;
    case GLFW_MOUSE_BUTTON_6:      return MouseButton::Button6;
    case GLFW_MOUSE_BUTTON_7:      return MouseButton::Button7;
    case GLFW_MOUSE_BUTTON_8:      return MouseButton::Button8;
    default:                       return MouseButton::Invalid;
    }
}

// ---------------------------------------------------------------------------
// WindowBackend
// ---------------------------------------------------------------------------

WindowBackend::WindowBackend(WindowsService* service,
    const std::string& title, Vec2i size, Vec2i position)
{
    static bool glfwInitialized = false;
    if (!glfwInitialized)
    {
        if (!glfwInit())
        {
            TWISTED_ERROR("glfwInit() failed");
            return;
        }
        glfwInitialized = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    glfwWindow = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
    if (!glfwWindow)
    {
        TWISTED_ERROR("glfwCreateWindow() failed");
        return;
    }

    glfwSetWindowPos(glfwWindow, position.x, position.y);
    glfwMakeContextCurrent(glfwWindow);

    // Store service pointer for callbacks via user pointer
    glfwSetWindowUserPointer(glfwWindow, service);

    // Load GLAD
    static bool gladLoaded = false;
    if (!gladLoaded)
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            TWISTED_ERROR("GLAD initialization failed");
            return;
        }
        gladLoaded = true;
    }

    // Release context so the render thread can acquire it
    glfwMakeContextCurrent(nullptr);

    // --- Callbacks ---

    glfwSetKeyCallback(glfwWindow,
        [](GLFWwindow* w, int key, int /*scancode*/, int action, int /*mods*/)
        {
            if (action == GLFW_REPEAT) return;
            auto* svc = static_cast<WindowsService*>(glfwGetWindowUserPointer(w));
            KeyEvent e;
            e.key   = glfwToTwistedKey(key);
            e.state = (action == GLFW_PRESS) ? KeyState::PRESSED : KeyState::RELEASED;
            svc->DispatchEvent(e);
        });

    glfwSetMouseButtonCallback(glfwWindow,
        [](GLFWwindow* w, int button, int action, int /*mods*/)
        {
            auto* svc = static_cast<WindowsService*>(glfwGetWindowUserPointer(w));
            MouseButtonEvent e;
            e.button = glfwToTwistedButton(button);
            e.state  = (action == GLFW_PRESS) ? KeyState::PRESSED : KeyState::RELEASED;
            svc->DispatchEvent(e);
        });

    glfwSetCursorPosCallback(glfwWindow,
        [](GLFWwindow* w, double x, double y)
        {
            auto* svc = static_cast<WindowsService*>(glfwGetWindowUserPointer(w));
            MouseMoveEvent e;
            e.position = { static_cast<float>(x), static_cast<float>(y) };
            svc->DispatchEvent(e);
        });

    glfwSetScrollCallback(glfwWindow,
        [](GLFWwindow* w, double /*xoff*/, double yoff)
        {
            auto* svc = static_cast<WindowsService*>(glfwGetWindowUserPointer(w));
            MouseWheelEvent e;
            e.delta = static_cast<float>(yoff);
            svc->DispatchEvent(e);
        });

    glfwSetWindowSizeCallback(glfwWindow,
        [](GLFWwindow* w, int width, int height)
        {
            auto* svc = static_cast<WindowsService*>(glfwGetWindowUserPointer(w));
            WindowResizeEvent e;
            e.size = { width, height };
            svc->DispatchEvent(e);
        });

    glfwSetWindowCloseCallback(glfwWindow,
        [](GLFWwindow* w)
        {
            auto* svc = static_cast<WindowsService*>(glfwGetWindowUserPointer(w));
            svc->DispatchEvent(WindowCloseEvent{});
        });

    glfwSetWindowFocusCallback(glfwWindow,
        [](GLFWwindow* w, int focused)
        {
            auto* svc = static_cast<WindowsService*>(glfwGetWindowUserPointer(w));
            svc->DispatchEvent(WindowFocusEvent{ focused == GLFW_TRUE });
        });

    glfwSetDropCallback(glfwWindow,
        [](GLFWwindow* w, int count, const char** paths)
        {
            auto* svc = static_cast<WindowsService*>(glfwGetWindowUserPointer(w));
            std::vector<std::filesystem::path> filePaths;
            filePaths.reserve(static_cast<size_t>(count));
            for (int i = 0; i < count; i++)
                filePaths.emplace_back(paths[i]);
            svc->DispatchEvent(FilesDroppedEvent{ std::move(filePaths) });
        });

    TWISTED_INFO("GLFW Window created");
}

WindowBackend::~WindowBackend()
{
    if (glfwWindow)
    {
        glfwDestroyWindow(glfwWindow);
        glfwWindow = nullptr;
    }
    glfwTerminate();
}

// ---------------------------------------------------------------------------
// Window
// ---------------------------------------------------------------------------

Window::Window(WindowsService* service, const std::string& title, Vec2i size, Vec2i position)
    : m_backend(std::make_unique<WindowBackend>(service, title, size, position))
    , m_windowsService(service)
{}

Window::~Window() = default;
Window::Window(Window&& other) = default;
Window& Window::operator=(Window&& other) = default;

Vec2i Window::GetSize() const
{
    if (!m_backend || !m_backend->glfwWindow) return {};
    Vec2i s{};
    glfwGetWindowSize(m_backend->glfwWindow, &s.x, &s.y);
    return s;
}

Vec2i Window::GetClientSize() const
{
    if (!m_backend || !m_backend->glfwWindow) return {};
    Vec2i s{};
    glfwGetFramebufferSize(m_backend->glfwWindow, &s.x, &s.y);
    return s;
}

Vec2i Window::GetPosition() const
{
    if (!m_backend || !m_backend->glfwWindow) return {};
    Vec2i p{};
    glfwGetWindowPos(m_backend->glfwWindow, &p.x, &p.y);
    return p;
}

std::string Window::GetTitle() const
{
    if (!m_backend || !m_backend->glfwWindow) return {};
    const char* t = glfwGetWindowTitle(m_backend->glfwWindow);
    return t ? t : "";
}

bool Window::IsWindowMaximized() const
{
    if (!m_backend || !m_backend->glfwWindow) return false;
    return glfwGetWindowAttrib(m_backend->glfwWindow, GLFW_MAXIMIZED) == GLFW_TRUE;
}

Window::RestoreBounds Window::GetRestoreBounds() const
{
    return { GetPosition(), GetSize() };
}

void Window::SetTitle(const std::string& newName)
{
    if (m_backend && m_backend->glfwWindow)
        glfwSetWindowTitle(m_backend->glfwWindow, newName.c_str());
}

void Window::SetSize(Vec2i newSize)
{
    if (m_backend && m_backend->glfwWindow)
        glfwSetWindowSize(m_backend->glfwWindow, newSize.x, newSize.y);
}

void Window::SetPosition(Vec2i newPosition)
{
    if (m_backend && m_backend->glfwWindow)
        glfwSetWindowPos(m_backend->glfwWindow, newPosition.x, newPosition.y);
}

void Window::Minimize()
{
    if (m_backend && m_backend->glfwWindow)
        glfwIconifyWindow(m_backend->glfwWindow);
}

void Window::Maximize()
{
    if (m_backend && m_backend->glfwWindow)
        glfwMaximizeWindow(m_backend->glfwWindow);
}

void Window::Restore()
{
    if (m_backend && m_backend->glfwWindow)
        glfwRestoreWindow(m_backend->glfwWindow);
}

void Window::SetFullScreen(bool isFullScreen)
{
    if (!m_backend || !m_backend->glfwWindow) return;
    if (isFullScreen)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(m_backend->glfwWindow, monitor,
            0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else
    {
        Vec2i pos  = GetPosition();
        Vec2i size = GetSize();
        glfwSetWindowMonitor(m_backend->glfwWindow, nullptr,
            pos.x, pos.y, size.x, size.y, GLFW_DONT_CARE);
    }
}

void Window::SetWindowed(Vec2i size, Vec2i pos)
{
    if (m_backend && m_backend->glfwWindow)
        glfwSetWindowMonitor(m_backend->glfwWindow, nullptr,
            pos.x, pos.y, size.x, size.y, GLFW_DONT_CARE);
}

bool Window::CloseWindow()
{
    if (!m_backend || !m_backend->glfwWindow) return false;
    glfwSetWindowShouldClose(m_backend->glfwWindow, GLFW_TRUE);
    TWISTED_INFO("Window closed");
    return true;
}

void Window::Clear(const Color& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::SetVSync(int deltaFrames)
{
    glfwSwapInterval(deltaFrames);
}

void Window::SwapBuffers()
{
    if (m_backend && m_backend->glfwWindow)
        glfwSwapBuffers(m_backend->glfwWindow);
}

void Window::AcquireGLContext()
{
    if (m_backend && m_backend->glfwWindow)
        glfwMakeContextCurrent(m_backend->glfwWindow);
}

void Window::ReleaseGLContext()
{
    glfwMakeContextCurrent(nullptr);
}

void* Window::GetRawPointer()
{
    return m_backend ? m_backend->glfwWindow : nullptr;
}

} // namespace Twisted
