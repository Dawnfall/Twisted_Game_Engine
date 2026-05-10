#pragma once

#include "AppCore.h"
#include "Utils/GlmUtils.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

namespace Twisted
{
    class WindowsService;

    struct WindowBackend
    {
        GLFWwindow* glfwWindow = nullptr;

        WindowBackend(WindowsService* service, const std::string& title, Vec2i size, Vec2i position);
        ~WindowBackend();
    };
}
