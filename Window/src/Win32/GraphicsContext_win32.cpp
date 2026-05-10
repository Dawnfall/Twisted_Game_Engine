#include "AppCore.h"
#include "RenderAPI.h"
#include "Debug/Logger.h"

#include <Windows.h>
#include <glad/glad.h>       // main GLAD functions
#include <GL/wglext.h>

namespace Twisted
{
    static bool setPixelFormat(HDC windowHandle)
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

    using PFNWGLCREATECONTEXTATTRIBSARBPROC = HGLRC(WINAPI*)(HDC, HGLRC, const int*);
    
    static HGLRC createContext(HDC hdc)
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

        auto loader = [](const char* name) -> void* {
            void* p = (void*)wglGetProcAddress(name);
            if (!p) p = (void*)GetProcAddress(GetModuleHandleA("opengl32.dll"), name);
            return p;
            };

        if (!gladLoadGLLoader(loader))
        {
            TWISTED_ERROR("GLAD initialization failed on temp context");

            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(realContext);
            return nullptr;
        }

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


	GraphicsContext::GraphicsContext(void* windowHandle) :m_windowHandle(windowHandle)
	{
        // 1. Get the device context for the window
        m_hdc = GetDC(static_cast<HWND>(m_windowHandle));
        if (!m_hdc)
        {
            TWISTED_ERROR("Failed to get device context (HDC) for window! GraphicsContext_win32 failed!");
            return;
        }

        // 2. Choose a pixel format for the device context
        if (!setPixelFormat(static_cast<HDC>(m_hdc)))
        {
            TWISTED_ERROR("GraphicsContext_win32 failed!");
            return;
        }

        // 3. Create the OpenGL rendering context
        m_glrc = createContext(static_cast<HDC>(m_hdc));
        if (!m_glrc)
        {
            TWISTED_ERROR("GraphicsContext_win32 failed!");
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
}
