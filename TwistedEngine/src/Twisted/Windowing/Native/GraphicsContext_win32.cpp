#include "AppCore.h"
#ifdef NATIVE_USE

#include "Twisted/Windowing/GraphicsContext.h"
#include "Debug/Logger.h"

#include <Windows.h>

namespace Twisted
{
	GraphicsContext::GraphicsContext(void* windowHandle) :m_windowHandle(windowHandle)
	{
        // 1. Get the device context for the window
        m_hdc = GetDC(static_cast<HWND>(m_windowHandle));
        if (!m_hdc)
        {
            TWISTED_ERROR("Failed to get device context (HDC) for window!");
            return;
        }

        // 2. Choose a pixel format for the device context
        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;

        int pixelFormat = ChoosePixelFormat(static_cast<HDC>(m_hdc), &pfd);
        if (pixelFormat == 0)
        {
            TWISTED_ERROR("ChoosePixelFormat() failed!");
            return;
        }

        if (!SetPixelFormat(static_cast<HDC>(m_hdc), pixelFormat, &pfd))
        {
            TWISTED_ERROR("SetPixelFormat() failed!");
            return;
        }

        // 3. Create the OpenGL rendering context
        m_glrc = wglCreateContext(static_cast<HDC>(m_hdc));
        if (!m_glrc)
        {
            TWISTED_ERROR("wglCreateContext() failed!");
            return;
        }

        // 4. Make the context current for this thread
        if (!wglMakeCurrent(static_cast<HDC>(m_hdc), static_cast<HGLRC>(m_hdc)))
        {
            TWISTED_ERROR("wglMakeCurrent() failed!");
            return;
        }

        // 5. Log success
        TWISTED_INFO("GraphicsContext_win32 successfully initialized!");
	}

    GraphicsContext::~GraphicsContext()
    {
        if (m_glrc)
        {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(static_cast<HGLRC>(m_glrc));
            m_glrc = nullptr;
        }

        if (m_hdc && m_windowHandle)
        {
            ReleaseDC(static_cast<HWND>(m_windowHandle), static_cast<HDC>(m_hdc));
            m_hdc = nullptr;
        }
    }

    void GraphicsContext::SwapBuffers()
    {
        if (!m_hdc)
        {
            TWISTED_WARN("GraphicsContext_win32::SwapBuffers(): HDC is null - probably the context wasn't initialized.");
            return;
        }

        // If using wgl and the context is made current on this thread you can directly swap the HDC.
        // SwapBuffers() returns nonzero on success.
        if (::SwapBuffers(static_cast<HDC>(m_hdc)) == FALSE)
        {
            DWORD err = ::GetLastError();
            TWISTED_ERROR("GraphicsContext_win32::SwapBuffers() failed. GetLastError() = " + std::to_string(static_cast<unsigned>(err)));
        }
    }

    void GraphicsContext::SetVSync(int deltaFrames)
    {
        // deltaFrames == 1 ? enable vsync
        // deltaFrames == 0 ? disable vsync

        typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);
        static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
            (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

        if (wglSwapIntervalEXT)
            wglSwapIntervalEXT(deltaFrames);
    }

    void GraphicsContext::Clear(const Color& color)
    {
/*        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)*/;
    }

}

#endif