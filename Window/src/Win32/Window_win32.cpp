#include "AppCore.h"

#include "WindowsService.h"
#include "Window.h"
#include "WIN32/Window_Win32.h"
#include "WIN32/WindowsUtils_Win32.h"
#include "Debug/Logger.h"

#include <Windows.h>
#include <windowsx.h>
#include <GL/wglext.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <tuple>
#include <memory>

using PFNWGLCREATECONTEXTATTRIBSARBPROC = HGLRC(WINAPI*)(HDC, HGLRC, const int*);
namespace Twisted
{
	Window::Window(WindowsService* service, const std::string& title, Vec2i size, Vec2i position) :
		m_windowsService(service),
		m_backend(std::make_unique<WindowBackend>(title,size,position))
	{
	}

	Window::~Window()
	{

	}

	Window::Window(Window&& other) = default;
	Window& Window::operator=(Window&& other) = default;

	WindowBackend::WindowBackend(const std::string& title, Vec2i& size, Vec2i position)
	{
		hwnd = createWindowHandle(title, size, position);
		if (!hwnd)
		{
			TWISTED_ERROR("Failed to create Win32 window!");
			return;
		}

		// 1. Get the device context for the window
		hdc = GetDC(hwnd);
		if (!hdc)
		{
			TWISTED_ERROR("Failed to get device context (HDC) for window! GraphicsContext_win32 failed!");
			return;
		}

		// 2. Choose a pixel format for the device context
		if (!setPixelFormat(hdc))
		{
			TWISTED_ERROR("GraphicsContext_win32 failed!");
			return;
		}

		// 3. Create the OpenGL rendering context
		glrc = createContext(hdc);
		if (!glrc)
		{
			TWISTED_ERROR("GraphicsContext_win32 failed!");
			return;
		}

		// 5. Log success
		TWISTED_INFO("GraphicsContext_win32 successfully initialized!");

		ShowWindow(hwnd, SW_SHOW);
		DragAcceptFiles(hwnd, TRUE);

		// Restore DWM drop shadow for borderless window
		MARGINS margins{ 1, 1, 1, 1 };
		DwmExtendFrameIntoClientArea(hwnd, &margins);

		TWISTED_INFO("Win32 Window Created");
	}
	WindowBackend::~WindowBackend()
	{
		if (hwnd)
			DestroyWindow(hwnd);

		if (glrc)
		{
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(glrc);
			glrc = nullptr;
		}

		if (hdc)
		{
			ReleaseDC(hwnd, hdc);
			hdc = nullptr;
		}
		hwnd = nullptr;
	}

	void Window::AcquireGLContext()
	{
		wglMakeCurrent(m_backend->hdc, m_backend->glrc);
	}

	void Window::ReleaseGLContext()
	{
		wglMakeCurrent(nullptr, nullptr);
	}

	void Window::SetVSync(int deltaFrames) 
	{
		// deltaFrames == 1 ? enable vsync
		// deltaFrames == 0 ? disable vsync

		typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);
		static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
			(PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		if (wglSwapIntervalEXT)
			wglSwapIntervalEXT(deltaFrames);
	}

	Vec2i Window::GetSize() const
	{
		RECT rect{};
		if (m_backend->hwnd)
			GetWindowRect(m_backend->hwnd, &rect);
		return Vec2i{ rect.right - rect.left, rect.bottom - rect.top };
	}

	Vec2i Window::GetClientSize() const
	{
		RECT rect{};
		if (m_backend->hwnd)
			GetClientRect(m_backend->hwnd, &rect);
		return Vec2i{ rect.right - rect.left, rect.bottom - rect.top };
	}

	Vec2i Window::GetPosition() const
	{
		RECT rect{};
		if (m_backend->hwnd)
			GetWindowRect(m_backend->hwnd, &rect);
		return Vec2i{ rect.left, rect.top };
	}

	std::string Window::GetTitle() const
	{
		char buffer[256]{};
		if (m_backend->hwnd)
			GetWindowTextA(m_backend->hwnd, buffer, 256);
		return std::string(buffer);
	}

	void Window::SetTitle(const std::string& newTitle)
	{
		if (m_backend->hwnd)
			SetWindowTextA(m_backend->hwnd, newTitle.c_str());
	}

	bool Window::IsWindowMaximized() const
	{
		return m_backend->hwnd && IsZoomed(m_backend->hwnd);
	}

	Window::RestoreBounds Window::GetRestoreBounds() const
	{
		WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
		if (m_backend->hwnd && GetWindowPlacement(m_backend->hwnd, &wp))
		{
			const RECT& r = wp.rcNormalPosition;
			return { Vec2i{r.left, r.top}, Vec2i{r.right - r.left, r.bottom - r.top} };
		}
		return { GetPosition(), GetSize() };
	}

	void Window::Minimize()
	{
		if (m_backend->hwnd)
			ShowWindow(m_backend->hwnd, SW_MINIMIZE);
	}

	void Window::Maximize()
	{
		if (m_backend->hwnd)
			ShowWindow(m_backend->hwnd, SW_SHOWMAXIMIZED);
	}

	void Window::Restore()
	{
		if (m_backend->hwnd)
			ShowWindow(m_backend->hwnd, SW_RESTORE);
	}

	void Window::SetFullScreen(bool isFullScreen)
	{
		if (!m_backend->hwnd)
			return;

		if (isFullScreen)
		{
			MONITORINFO mi = { sizeof(mi) };
			if (GetMonitorInfo(MonitorFromWindow(m_backend->hwnd, MONITOR_DEFAULTTOPRIMARY), &mi))
			{
				SetWindowPos(m_backend->hwnd, HWND_TOP,
					mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
		}
		else
		{
			SetWindowPos(m_backend->hwnd, HWND_NOTOPMOST, 100, 100, 1280, 720,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}

	void Window::SetSize(Vec2i newSize)
	{
		if (m_backend->hwnd)
			SetWindowPos(static_cast<HWND>(m_backend->hwnd), nullptr, 0, 0, newSize.x, newSize.y, SWP_NOMOVE | SWP_NOZORDER);
	}

	void Window::SetPosition(Vec2i newPosition)
	{
		if (m_backend->hwnd)
			SetWindowPos(static_cast<HWND>(m_backend->hwnd), nullptr, newPosition.x, newPosition.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	void Window::SetWindowed(Vec2i size, Vec2i pos)
	{
		SetSize(size);
		SetPosition(pos);
	}

	bool Window::CloseWindow()
	{
		if (m_backend->hwnd)
		{
			PostMessage(static_cast<HWND>(m_backend->hwnd), WM_CLOSE, 0, 0);
			TWISTED_INFO("Window closed");
			return true;
		}
		return false;
	}

	void* Window::GetRawPointer()
	{
		return m_backend->hwnd;
	}

	void Window::Clear(const Color& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::SwapBuffers()
	{
		if (!m_backend->hdc)
		{
			TWISTED_WARN("GraphicsContext_win32::SwapBuffers(): HDC is null - probably the context wasn't initialized.");
			return;
		}

		// If using wgl and the context is made current on this thread you can directly swap the HDC.
		// SwapBuffers() returns nonzero on success.
		if (::SwapBuffers(m_backend->hdc) == FALSE)
		{
			DWORD err = ::GetLastError();
			TWISTED_ERROR("GraphicsContext_win32::SwapBuffers() failed. GetLastError() = " + std::to_string(static_cast<unsigned>(err)));
		}
	}

	HGLRC createContext(HDC hdc)
	{
		if (!hdc)
			return nullptr;

		// -------------------------
		// 1) Create temporary legacy context
		// -------------------------
		HGLRC tempContext = wglCreateContext(hdc);
		if (!tempContext)
		{
			TWISTED_ERROR("wglCreateContext (temp) failed");
			return nullptr;
		}

		if (!wglMakeCurrent(hdc, tempContext))
		{
			TWISTED_ERROR("wglMakeCurrent (temp) failed");
			wglDeleteContext(tempContext);
			return nullptr;
		}


		// -------------------------
		// 2) Load wglCreateContextAttribsARB
		// -------------------------
		auto wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(
			wglGetProcAddress("wglCreateContextAttribsARB")
			);

		HGLRC realContext = nullptr;

		if (wglCreateContextAttribsARB)
		{
			// -------------------------
			// 3) Modern OpenGL 4.6 core context attributes
			// -------------------------
			const int attribs[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
				WGL_CONTEXT_MINOR_VERSION_ARB, 6,
				WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0
			};

			realContext = wglCreateContextAttribsARB(hdc, 0, attribs);
			if (!realContext)
			{
				TWISTED_WARN("wglCreateContextAttribsARB failed; falling back to legacy context");
				realContext = tempContext;
			}
			else
			{
				// Delete temporary context
				wglMakeCurrent(nullptr, nullptr);
				wglDeleteContext(tempContext);
			}
		}
		else
		{
			TWISTED_WARN("wglCreateContextAttribsARB not available; using legacy context");
			realContext = tempContext;
		}

		// -------------------------
		// 4) Make the chosen context current
		// -------------------------
		if (!wglMakeCurrent(hdc, realContext))
		{
			TWISTED_ERROR("wglMakeCurrent failed for final context");
			wglDeleteContext(realContext);
			return nullptr;
		}


		// -------------------------
		// 2) Load GL functions using GLAD 
		// -------------------------

		static bool gladLoaded = false;
		if (!gladLoaded)
		{
			auto loader = [](const char* name) -> void* {
				void* p = (void*)wglGetProcAddress(name);
				if (!p) p = (void*)GetProcAddress(GetModuleHandleA("opengl32.dll"), name);
				return p;
			};

			if (!gladLoadGLLoader(loader))
			{
				TWISTED_ERROR("GLAD initialization failed");
				wglMakeCurrent(nullptr, nullptr);
				wglDeleteContext(realContext);
				return nullptr;
			}
			gladLoaded = true;
		}

		// Release context so the render thread can acquire it
		wglMakeCurrent(nullptr, nullptr);

		// -------------------------
		// 6) Optional: log OpenGL version
		// -------------------------
		//const GLubyte* version = glGetString(GL_VERSION);
		//if (version)
		//{
		//    std::string openglVersion(reinterpret_cast<const char*>(version));
		//    TWISTED_INFO("OpenGL Version: " + openglVersion);
		//}
		//else
		//{
		//    TWISTED_WARN("glGetString(GL_VERSION) returned null!");
		//}
		return realContext;
	}

	bool setPixelFormat(HDC windowHandle)
	{
		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pixelFormat = ChoosePixelFormat(windowHandle, &pfd);
		if (pixelFormat == 0)
		{
			TWISTED_ERROR("ChoosePixelFormat() failed!");
			return false;
		}

		PIXELFORMATDESCRIPTOR chosenPFD;
		if (!DescribePixelFormat(windowHandle, pixelFormat, sizeof(chosenPFD), &chosenPFD))
		{
			TWISTED_ERROR("DescribePixelFormat() failed!");
			return false;
		}

		if (!SetPixelFormat(windowHandle, pixelFormat, &pfd))
		{
			TWISTED_ERROR("SetPixelFormat() failed!");
			return false;
		}

		return true;
	}

	HWND createWindowHandle(const std::string& title, Vec2i size, Vec2i position)
	{
		WNDCLASSEX wc{};
		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.lpfnWndProc   = WndProc;
		wc.hInstance     = GetModuleHandle(nullptr);
		wc.lpszClassName = WINDOW_CLASS_NAME.c_str();
		wc.style         = CS_OWNDC;
		wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);

		if (!RegisterClassEx(&wc))
		{
			DWORD err = GetLastError();
			if (err != ERROR_CLASS_ALREADY_EXISTS)
			{
				TWISTED_ERROR("Failed to register window class. Error: {}", err);
				return nullptr;
			}
		}

		HWND hwnd = CreateWindowEx(
			0,
			wc.lpszClassName,
			title.c_str(),
			WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
			position.x, position.y, size.x, size.y,
			nullptr, nullptr,
			wc.hInstance,
			nullptr
		);

		if (!hwnd)
		{
			TWISTED_ERROR("CreateWindowEx failed. Error: {}", GetLastError());
			return nullptr;
		}

		return hwnd;
	}
}

